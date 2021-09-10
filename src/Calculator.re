[@react.component]
let make = () => {
  let display = "TODO";
  <section id="calculator">
    <div className="display"> {ReasonReact.string(display)} </div>
    <section className="buttons">
      <section className="numGrid">
        <button id="clear" onClick={_ => ignore() /* TODO */}>
          {ReasonReact.string("AC")}
        </button>
        <button id="eval" onClick={_ => ignore() /* TODO */}>
          {ReasonReact.string("=")}
        </button>
        {Belt.Array.map(
           [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9|],
           digit => {
             let digitAsString = string_of_int(digit);
             <button
               className="digit"
               key={"num-" ++ digitAsString}
               onClick={_ => ignore() /* TODO */}>
               {ReasonReact.string(digitAsString)}
             </button>;
           },
         )
         ->ReasonReact.array}
      </section>
      <section className="ops">
        <button className="op" onClick={_ => ignore() /* TODO */}>
          {ReasonReact.string("+")}
        </button>
        <button className="op" onClick={_ => ignore() /* TODO */}>
          {ReasonReact.string("-")}
        </button>
        <button className="op" onClick={_ => ignore() /* TODO */}>
          {ReasonReact.string("x")}
        </button>
        <button className="op" onClick={_ => ignore() /* TODO */}>
          {ReasonReact.string("/")}
        </button>
      </section>
    </section>
  </section>;
};
