let component = ReasonReact.statelessComponent("background");

let make = children => {
  ...component,
  render: _self =>
    <Theme.Context.Consumer>
      ...(
           theme =>
             ReasonReact.createDomElement(
               "div",
               ~props={
                 "className":
                   theme === Light ? "background-light" : "background-dark",
               },
               children,
             )
         )
    </Theme.Context.Consumer>,
};
