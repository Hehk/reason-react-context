type t =
  | Light
  | Dark;

module Context =
  ReasonReactContext.CreateContext(
    {
      type state = t;
      let name = "Theme";
      let defaultValue = Light;
    },
  );
