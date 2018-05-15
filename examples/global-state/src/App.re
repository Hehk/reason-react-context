let component = ReasonReact.statelessComponent("App");

module TestComponent = {
  let component = ReasonReact.statelessComponent("TestComponent");
  let make = _children => {
    ...component,
    render: _self => {
      /* This component should only render once (on load), this is here to document that the component-tree only */
      /* causes a re-render in consumers */
      Js.log("Initial Render");
      <div> <Input /> <AddInput /> <TodoList /> </div>;
    },
  };
};

let make = _children => {
  ...component,
  render: _self => <State> <TestComponent /> </State>,
};
