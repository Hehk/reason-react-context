/* This allows for children pass through without coupling to the DOM with ReasonReact.createDomElement */
let passThrough: ReasonReact.reactClass = [%bs.raw
  {| props => props.children |}
];

let make = children =>
  ReasonReact.wrapJsForReason(
    ~reactClass=passThrough,
    ~props=Js.Obj.empty(),
    children
  );
