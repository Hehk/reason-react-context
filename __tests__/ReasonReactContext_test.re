open Jest;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

type counterState = {count: int};

describe("Creating and updating", () => {
  open Expect;
  module Context =
    ReasonReactContext.CreateContext(
      {
        type state = counterState;
        let name = "Counter";
        let defaultValue = {count: 0};
      }
    );
  test("Provider renders", () =>
    <Context.Provider /> |> Enzyme.shallow |> Enzyme.exists |> expect |> toBe(true)
  );
  test("Consumer renders", () => {
    let x = ref(-1);
    let _ =
      <Context.Consumer>
        ...(
             ({count}) => {
               x := count;
               ReasonReact.stringToElement("test");
             }
           )
      </Context.Consumer>
      |> Enzyme.shallow;
    x^ |> expect |> toBe(0);
  });
  test("Change count on provider and it is mirrored in consumer", () => {
    let x = ref(-1);
    let newValue = 10;
    let _ = <Context.Provider value={count: newValue} /> |> Enzyme.shallow;
    let _ =
      <Context.Consumer>
        ...(
             ({count}) => {
               x := count;
               ReasonReact.stringToElement("test");
             }
           )
      </Context.Consumer>
      |> Enzyme.shallow;
    x^ |> expect |> toBe(newValue);
  });
});
