type state = {
  register: int,
  maybeLastOp: option(Operator.t),
  exprs: list(expr),
}

and expr =
  | Op(Operator.t)
  | Eval
  | Digit(int);

let extractInputs = (~lsd as maybeLSD=?, exprs: list(expr)) => {
  let initDigits =
    switch (maybeLSD) {
    | Some(lsd) => [lsd]
    | None => []
    };
  let (_shouldEval, digits) =
    exprs->Belt.List.reduce(
      (false, initDigits), ((shouldEval, digits), expr) =>
      shouldEval
        ? (true, digits)
        : (
          switch (expr) {
          | Digit(n) => (false, digits->Belt.List.add(n))
          | _ => (true, digits)
          }
        )
    );
  switch (digits) {
  | [] => None
  | _ => Some(digits->Belt.List.reduce(0, (acc, digit) => acc * 10 + digit))
  };
};

let initState = () => {register: 0, maybeLastOp: None, exprs: []};

[@react.component]
let make = () => {
  let ({register, exprs}, setState) = React.useState(() => initState());
  let display =
    switch (exprs) {
    | [Digit(lsd), ...remainderExprs] =>
      remainderExprs
      |> extractInputs(~lsd)
      |> Belt.Option.getExn
      |> string_of_int
    | []
    | [Op(_), ..._]
    | [Eval, ..._] => string_of_int(register)
    };

  let nextRegister = (~expr, {register, maybeLastOp, exprs}) =>
    switch (expr) {
    | Eval
    | Op(_) =>
      switch (extractInputs(exprs)) {
      | Some(input) =>
        switch (maybeLastOp) {
        | Some(Add) => register + input
        | Some(Minus) => register - input
        | Some(Multiply) => register * input
        | Some(Divide) => register / input
        | None => input
        }
      | None => register
      }
    | Digit(_) => register
    };

  let onExpr = expr => {
    setState(state =>
      {
        register: nextRegister(~expr, state),
        maybeLastOp:
          switch (expr) {
          | Op(op) => Some(op)
          | Eval => None
          | Digit(_) => state.maybeLastOp
          },
        exprs: state.exprs->Belt.List.add(expr),
      }
    );
  };

  <section id="calculator">
    <div className="display"> {ReasonReact.string(display)} </div>
    <section className="buttons">
      <section className="numGrid">
        <button id="clear" onClick={_ => setState(_ => initState())}>
          {ReasonReact.string("AC")}
        </button>
        <button id="eval" onClick={_ => onExpr(Eval)}>
          {ReasonReact.string("=")}
        </button>
        {Belt.Array.map(
           [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9|],
           digit => {
             let digitAsString = string_of_int(digit);
             <button
               className="digit"
               key={"num-" ++ digitAsString}
               onClick={_ => onExpr(Digit(digit))}>
               {ReasonReact.string(digitAsString)}
             </button>;
           },
         )
         ->ReasonReact.array}
      </section>
      <section className="ops">
        <button className="op" onClick={_ => onExpr(Op(Add))}>
          {ReasonReact.string("+")}
        </button>
        <button className="op" onClick={_ => onExpr(Op(Minus))}>
          {ReasonReact.string("-")}
        </button>
        <button className="op" onClick={_ => onExpr(Op(Multiply))}>
          {ReasonReact.string("x")}
        </button>
        <button className="op" onClick={_ => onExpr(Op(Divide))}>
          {ReasonReact.string("/")}
        </button>
      </section>
    </section>
  </section>;
};
