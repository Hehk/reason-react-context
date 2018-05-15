let component = ReasonReact.statelessComponent("TodoList");

let make = _children => {
  ...component,
  render: _self =>
    <State.Consumer>
      ...(
           ({state, send}) =>
             <ol>
               (
                 state.todos
                 |> Belt.List.reverse
                 |> Belt.List.map(_, todo =>
                      <li
                        key=(string_of_int(todo.id))
                        onClick=(_e => send(ToggleTodo(todo.id)))
                        style=(
                          ReactDOMRe.Style.make(
                            ~textDecoration=
                              todo.complete ? "line-through" : "",
                            (),
                          )
                        )>
                        (ReasonReact.string(todo.content))
                      </li>
                    )
                 |> Belt.List.toArray
                 |> ReasonReact.array
               )
             </ol>
         )
    </State.Consumer>,
};
