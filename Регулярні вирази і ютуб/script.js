let regex = /(?:https?:\/\/)?(?:www\.)?youtu(?:\.be\/|be.com\/\S*(?:watch|embed)(?:(?:(?=\/[-a-zA-Z0-9_]{11,}(?!\S))\/)|(?:\S*v=|v\/)))([-a-zA-Z0-9_]{11,})/;

let text = prompt("Посилання на відео з ютубу");
let match = text.match(regex);

if (match) {
	let videoId = match[1];

	let Code = `<iframe width="800" height="500" 
	src="https://www.youtube.com/embed/${videoId}" </iframe>`

	document.write(Code);
}

/*////---Метод 2--- (мій)////
let text = prompt("Посилання на відео з ютубу");
let id = "";

let n = 0;
for (let i = text.length - 1; i >=0; i--) {
	if (text[i] == '?') { n = i + 3; break; }
}
for (let i = n; i < text.length; i++) {
	if (text[i] != '&') id += text[i];
	else break;
}

let Code = `<iframe width="800" height="500" 
src="https://www.youtube.com/embed/${id}" </iframe>`
document.write(Code);*/