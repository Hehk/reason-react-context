module type Config = {type state; let name: string; let defaultValue: state;};

module CreateContext = (C: Config) => {
  type action =
    | ChangeState(C.state);
  let state = ref(C.defaultValue);
  let subscriptions = ref([]);
  let addSubscription = f => {
    subscriptions := [f, ...subscriptions^];
    () => subscriptions := List.filter(sub => sub !== f, subscriptions^);
  };
  let updateState = newStateOpt => {
    let newState =
      switch newStateOpt {
      | None => C.defaultValue
      | Some(newValue) => newValue
      };
    state := newState;
    List.iter(f => f(newState), subscriptions^);
  };
  module Provider = {
    let component =
      ReasonReact.statelessComponent(C.name ++ "ContextProvider");
    let make = (~value=?, children) => {
      ...component,
      willReceiveProps: _self => updateState(value),
      didMount: _self => {
        updateState(value);
        ReasonReact.NoUpdate;
      },
      render: _self =>
        ReasonReact.createDomElement("div", ~props=Js.Obj.empty(), children)
    };
  };
  module Consumer = {
    let component = ReasonReact.reducerComponent(C.name ++ "ContextConsumer");
    let make = children => {
      ...component,
      initialState: () => state^,
      reducer: (action, _state) =>
        switch action {
        | ChangeState(newState) => ReasonReact.Update(newState)
        },
      subscriptions: ({send}) => [
        Sub(
          () => addSubscription(newState => send(ChangeState(newState))),
          unSub => unSub()
        )
      ],
      render: ({state}) => children(state)
    };
  };
};
