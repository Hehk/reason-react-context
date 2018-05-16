module type ContextConfig = {let debugName: string; type t; let value: t;};

module CreateContext:
  (C: ContextConfig) =>
  {
    type action =
      | ChangeState(C.t);
    let subscriptions: ref(array(C.t => unit));
    module Provider: {
      let make:
        (~value: C.t=?, array(ReasonReact.reactElement)) =>
        ReasonReact.componentSpec(
          ReasonReact.stateless,
          ReasonReact.stateless,
          ReasonReact.noRetainedProps,
          ReasonReact.noRetainedProps,
          ReasonReact.actionless,
        );
    };
    module Consumer: {
      let make:
        (C.t => ReasonReact.reactElement) =>
        ReasonReact.componentSpec(
          C.t,
          C.t,
          ReasonReact.noRetainedProps,
          ReasonReact.noRetainedProps,
          action,
        );
    };
  };
