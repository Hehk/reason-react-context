This a an implementation of the new context react context api in pure ReasonML

# State

It seems to cover the base api. If there is something missing, please file an issue :D

# Install

```
npm install --save reason-react-context
```

# Example

An example of using this is at https://github.com/Hehk/example-reason-react-context

Creating a context:

```re
type t =
  | Light
  | Dark;

module Context =
  ReasonReactContext.CreateContext(
    {
      type state = t;
      let name = "Theme";
      let defaultValue = Light;
    }
  );
```

Using the Provider module:

```re
type action =
  | ChangeTheme(Theme.t);

type state = {theme: Theme.t};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {theme: Light},
  reducer: (action, _state) =>
    switch action {
    | ChangeTheme(newTheme) => ReasonReact.Update({theme: newTheme})
    },
  render: ({send, state}) =>
    <div className="App">
      <Theme.Context.Provider value=state.theme>
        <Background>
          <button onClick=(_e => send(ChangeTheme(state.theme === Dark ? Light : Dark)))>
            (ReasonReact.stringToElement("Toggle Theme"))
          </button>
          <Title message="Reason Context" />
        </Background>
      </Theme.Context.Provider>
    </div>
};
```

Using the Consumer module:

```re
let component = ReasonReact.statelessComponent("background");

let make = children => {
  ...component,
  render: _self =>
    <Theme.Context.Consumer>
      ...(
           theme =>
             ReasonReact.createDomElement(
               "div",
               ~props={"className": theme === Light ? "background-light" : "background-dark"},
               children
             )
         )
    </Theme.Context.Consumer>
};
```
