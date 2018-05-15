type todo = {
  content: string,
  complete: bool,
  id: int,
};

type state = {
  todos: list(todo),
  input: string,
};

type action =
  | ChangeInput(string)
  | AddTodo
  | ToggleTodo(int);

type value = {
  send: action => unit,
  state,
};

let initialState = () => {todos: [], input: ""};

module Context =
  ReasonReactContext.CreateContext(
    {
      type state = value;
      let name = "GlobalState";
      let defaultValue = {
        send: _action => Js.log("State has not mounted yet"),
        state: initialState(),
      };
    },
  );

let component = ReasonReact.reducerComponent("State");

let make = children => {
  ...component,
  initialState,
  reducer: (action, state) =>
    switch (action) {
    | ChangeInput(input) => ReasonReact.Update({...state, input})
    | AddTodo =>
      ReasonReact.Update({
        todos: [
          {content: state.input, complete: false, id: Utils.uniqueId()},
          ...state.todos,
        ],
        input: "",
      })
    | ToggleTodo(id) =>
      ReasonReact.Update({
        ...state,
        todos:
          state.todos
          |> Belt.List.map(_, todo =>
               id === todo.id ? {...todo, complete: ! todo.complete} : todo
             ),
      })
    },
  render: ({state, send}) =>
    ReasonReact.element(
      Context.Provider.make(~value={state, send}, children),
    ),
};

/* Moving the consumer down in namespace */
module Consumer = Context.Consumer;
