type themes =
  | Light
  | Dark;

module Context =
  ReasonReactContext.CreateContext(
    {
      type t = themes;
      let debugName = "Theme";
      let value = Light;
    },
  );
