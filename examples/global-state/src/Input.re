let component = ReasonReact.statelessComponent("Input");

let handleChange = (send, e) => {
  let event = Obj.magic(e);
  send(State.ChangeInput(event##target##value));
};

let make = _children => {
  ...component,
  render: _self =>
    <State.Consumer>
      ...(
           ({state, send}) =>
             <input value=state.input onChange=(handleChange(send)) />
         )
    </State.Consumer>,
};
