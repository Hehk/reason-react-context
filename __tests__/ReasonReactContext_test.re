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
    <Context.Provider />
    |> Enzyme.shallow
    |> Enzyme.exists
    |> expect
    |> toBe(true)
  );
  let consumerCount = ref(-1);
  let consumer =
    <Context.Consumer>
      ...(
           ({count}) => {
             consumerCount := count;
             ReasonReact.stringToElement("test");
           }
         )
    </Context.Consumer>
    |> Enzyme.shallow;
  test("Consumer renders", () =>
    consumerCount^ |> expect |> toBe(0)
  );
  test("Change count on provider and it is mirrored in consumer", () => {
    let newValue = 10;
    let _ = <Context.Provider value={count: newValue} /> |> Enzyme.shallow;
    consumerCount^ |> expect |> toBe(newValue);
  });
  test("context subscription count should only be 1", () =>
    Context.subscriptions^ |> List.length |> expect |> toBe(1)
  );
  test("Consumer unmounts and the context clears the subscription", () => {
    let _ = consumer |> Enzyme.unmount;
    Context.subscriptions^ |> List.length |> expect |> toBe(0);
  });
});
