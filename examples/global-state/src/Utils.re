let uniqueId = {
  let idCounter = ref(0);
  () => {
    idCounter := idCounter^ + 1;
    idCounter^;
  };
};
