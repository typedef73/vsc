let N_floor = prompt("Введіть кількість поверхів");
let N_rooms = prompt("Введіть кількість квартир на поверсі");
let room = prompt("Введіть номер квартири");

let entrance = Math.ceil(room / (N_floor * N_rooms));

let floor = Math.ceil((room % (N_floor * N_rooms)) / N_rooms);

alert(`Під'їзд номер ${entrance}\nПоверх номер ${floor}`);