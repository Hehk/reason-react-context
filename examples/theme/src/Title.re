let component = ReasonReact.statelessComponent("Title");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <Theme.Context.Consumer>
      ...(
           theme =>
             <h1 className=(theme === Light ? "title-light" : "title-dark")>
               (ReasonReact.string(message))
             </h1>
         )
    </Theme.Context.Consumer>,
};
