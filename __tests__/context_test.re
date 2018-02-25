open Jest;

describe("sample test", () =>
  Expect.(test("1 == 1", () =>
            1 |> expect |> toBe(1)
          ))
);
