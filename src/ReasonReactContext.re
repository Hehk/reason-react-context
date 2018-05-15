module type Config = {type state; let name: string; let defaultValue: state;};

/* This allows for children pass through without coupling to the DOM with ReasonReact.createDomElement */
module RenderChildren = {
  let passThrough: ReasonReact.reactClass = [%bs.raw
    {| props => props.children |}
  ];
  let make = children =>
    ReasonReact.wrapJsForReason(
      ~reactClass=passThrough,
      ~props=Js.Obj.empty(),
      children,
    );
};

module CreateContext = (C: Config) => {
  type action =
    | ChangeState(C.state);
  let state = ref(C.defaultValue);
  let subscriptions = ref([||]);
  let addSubscription = f => {
    subscriptions := Js.Array.concat([|f|], subscriptions^);
    () => subscriptions := Js.Array.filter(sub => sub !== f, subscriptions^);
  };
  let updateState = newStateOpt => {
    let newState =
      switch (newStateOpt) {
      | None => C.defaultValue
      | Some(newValue) => newValue
      };
    state := newState;
    Js.Array.forEach(f => f(newState), subscriptions^);
  };
  module Provider = {
    let component =
      ReasonReact.statelessComponent(C.name ++ "ContextProvider");
    let make = (~value=?, children) => {
      ...component,
      shouldUpdate: _self => false,
      willReceiveProps: _self => updateState(value),
      didMount: _self => {
        updateState(value);
        ();
      },
      render: _self => <RenderChildren> ...children </RenderChildren>,
    };
  };
  module Consumer = {
    let component = ReasonReact.reducerComponent(C.name ++ "ContextConsumer");
    let make = children => {
      ...component,
      initialState: () => state^,
      reducer: (action, _state) =>
        switch (action) {
        | ChangeState(newState) => ReasonReact.Update(newState)
        },
      didMount: ({send, onUnmount}) =>
        (newState => send(ChangeState(newState)))
        |> addSubscription
        |> onUnmount,
      render: ({state}) => children(state),
    };
  };
};
