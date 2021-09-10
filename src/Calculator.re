type operation =
  | Add
  | Subtract
  | Multiply
  | Divide
  | None;

type state = {
  accumulator: float,
  value: int,
  operation,
  evaluated: bool,
};

let initialState = {
  accumulator: 0.0,
  value: 0,
  operation: None,
  evaluated: false,
};

let evaluate = (accumulator, operation, value) => {
  switch (operation) {
  | None => float_of_int(value)
  | Add => accumulator +. float_of_int(value)
  | Subtract => accumulator -. float_of_int(value)
  | Multiply => accumulator *. float_of_int(value)
  | Divide => accumulator /. float_of_int(value)
  };
};

let operate = (state, operation) =>
  if (state.evaluated) {
    {operation, value: 0, accumulator: state.accumulator, evaluated: false};
  } else {
    {
      operation,
      value: 0,
      accumulator: evaluate(state.accumulator, state.operation, state.value),
      evaluated: false,
    };
  };

[@react.component]
let make = () => {
  let (state, setState) = React.useState(_ => initialState);

  let display =
    if (state.evaluated) {
      "="
      ++ Js.Float.toString(
           Js.Math.round(state.accumulator *. 1000000.0) /. 1000000.0,
         );
    } else {
      string_of_int(state.value);
    };

  <section id="calculator">
    <div className="display"> {ReasonReact.string(display)} </div>
    <section className="buttons">
      <section className="numGrid">
        <button id="clear" onClick={_ => {setState(_ => initialState)}}>
          {ReasonReact.string("AC")}
        </button>
        <button
          id="eval"
          onClick={_ =>
            setState(oldState =>
              {
                ...oldState,
                accumulator:
                  evaluate(
                    oldState.accumulator,
                    oldState.operation,
                    oldState.value,
                  ),
                evaluated: true,
              }
            )
          }>
          {ReasonReact.string("=")}
        </button>
        {Belt.Array.map(
           [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9|],
           digit => {
             let digitAsString = string_of_int(digit);
             <button
               className="digit"
               key={"num-" ++ digitAsString}
               onClick={_ =>
                 setState(oldState =>
                   if (oldState.evaluated) {
                     {
                       ...oldState,
                       operation: None,
                       value: digit,
                       evaluated: false,
                     };
                   } else {
                     {...oldState, value: oldState.value * 10 + digit};
                   }
                 )
               }>
               {ReasonReact.string(digitAsString)}
             </button>;
           },
         )
         ->ReasonReact.array}
      </section>
      <section className="ops">
        <button
          className="op"
          onClick={_ => setState(oldState => operate(oldState, Add))}>
          {ReasonReact.string("+")}
        </button>
        <button
          className="op"
          onClick={_ => setState(oldState => operate(oldState, Subtract))}>
          {ReasonReact.string("-")}
        </button>
        <button
          className="op"
          onClick={_ => setState(oldState => operate(oldState, Multiply))}>
          {ReasonReact.string("x")}
        </button>
        <button
          className="op"
          onClick={_ => setState(oldState => operate(oldState, Divide))}>
          {ReasonReact.string("/")}
        </button>
      </section>
    </section>
  </section>;
};
