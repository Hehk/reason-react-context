type action =
  | ChangeTheme(Theme.t);

type state = {theme: Theme.t};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {theme: Light},
  reducer: (action, _state) =>
    switch (action) {
    | ChangeTheme(newTheme) => ReasonReact.Update({theme: newTheme})
    },
  render: ({send, state}) =>
    <div className="App">
      <Theme.Context.Provider value=state.theme>
        <Background>
          <button
            onClick=(
              _e => send(ChangeTheme(state.theme === Dark ? Light : Dark))
            )>
            (ReasonReact.string("Toggle Theme"))
          </button>
          <Title message="Reason Context" />
        </Background>
      </Theme.Context.Provider>
    </div>,
};
