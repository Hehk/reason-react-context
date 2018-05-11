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
  let subscriptions = ref([]);
  let addSubscription = f => {
    subscriptions := [f, ...subscriptions^];
    () => subscriptions := List.filter(sub => sub !== f, subscriptions^);
  };
  let updateState = newStateOpt => {
    let newState =
      switch (newStateOpt) {
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
      subscriptions: ({send}) => [
        Sub(
          () => addSubscription(newState => send(ChangeState(newState))),
          unSub => unSub(),
        ),
      ],
      render: ({state}) => children(state),
    };
  };
};

module type ConfigWithAction = {
  type state;
  type action;
  let name: string;
  let defaultValue: state;
};

module CreateContextWithSendConsumption = (C: ConfigWithAction) => {
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
      switch (newStateOpt) {
      | None => C.defaultValue
      | Some(newValue) => newValue
      };
    state := newState;
    List.iter(f => f(newState), subscriptions^);
  };
  let containerSend = ref(None);
  let setContainerSend = (send: option(C.action => unit)) =>
    containerSend := send;
  module Provider = {
    let component =
      ReasonReact.statelessComponent(C.name ++ "ContextProvider");
    let make = (~value=?, ~send: C.action => unit, children) => {
      ...component,
      willReceiveProps: _self => {
        setContainerSend(Some(send));
        updateState(value);
      },
      didMount: _self => {
        setContainerSend(Some(send));
        updateState(value);
        ReasonReact.NoUpdate;
      },
      render: _self => <RenderChildren> ...children </RenderChildren>,
    };
  };
  module Consumer = {
    let component =
      ReasonReact.reducerComponent(C.name ++ "ContextConsumerWithSelf");
    let make = children => {
      ...component,
      initialState: () => state^,
      reducer: (action, _state) =>
        switch (action) {
        | ChangeState(newState) => ReasonReact.Update(newState)
        },
      subscriptions: ({send}) => [
        Sub(
          () => addSubscription(newState => send(ChangeState(newState))),
          unSub => unSub(),
        ),
      ],
      render: ({state}) =>
        switch (containerSend^) {
        | Some(send) => children(state, send)
        | None => ReasonReact.nullElement
        },
    };
  };
};
