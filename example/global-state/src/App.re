let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <State> <Input /> <AddInput /> <TodoList /> </State>,
};
