const post = document.querySelector('button');
const unread_color = "hotpink";
const read_color = "deeppink";
var boddd = document.getElementById('twidls');

post.addEventListener('click', postTwidl);

function postTwidl() {
  var input = document.getElementById('twiput');
  if (input.value.length < 1 || input.value.length > 140) {
    Swal.fire({
      title: 'Error',
      text: 'Twidls must contain between 1 and 140 characters.',
      type: 'error'
    });
    input.value = null;
  } else {
    var twidl = document.createElement("DIV");
    var innertwid = document.createElement("P");
    var checker = document.createElement("INPUT");
    checker.type = 'checkbox';
    checker.className = "twidlcheck";
    checker.addEventListener('change', read_switcher);
    twidl.className = "twidl";
    innertwid.innerText = input.value;
    console.log(input);
    twidl.appendChild(innertwid);
    twidl.appendChild(checker);
    document.getElementById("twidlst").childNodes === [] ?
      document.getElementById("twidlst").appendChild(twidl) :
      document.getElementById("twidlst").insertBefore(twidl, document.getElementById("twidlst").childNodes[0])
    input.value = null;
  }
}

function read_switcher() {
  if (this.checked) {
    this.parentNode.style.backgroundColor = read_color;
    this.parentNode.style.opacity = 0.3;
  } else {
    this.parentNode.style.backgroundColor = unread_color;
    this.parentNode.style.opacity = 1;
  }
}
