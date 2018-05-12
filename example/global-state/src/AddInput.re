let component = ReasonReact.statelessComponent("AddInput");

let make = _children => {
  ...component,
  render: _self =>
    <State.Consumer>
      ...(
           ({send}) =>
             <button onClick=(_e => send(State.AddTodo))>
               (ReasonReact.string("Add"))
             </button>
         )
    </State.Consumer>,
};
