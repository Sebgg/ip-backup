const url = "http://localhost:8888/";

var create_twidl = function(msgId, msgText, flagged) {
  var checker = document.createElement("INPUT");
  checker.type = 'checkbox';
  checker.className = "twidlcheck";

  var div = $("<div></div>").text(msgText)
                            .addClass("twidl")
                            .attr("id", msgId);


  if (flagged === true) {
    div.addClass("read");
    checker.checked = true;
  }
  div.append(checker);
  $("#twidlst").prepend(div);
}

$(document).ready(function() {
  var xhr = new XMLHttpRequest();

  xhr.onreadystatechange = function() {
    if (this.readyState === 4 && this.status === 200) {
      const data = JSON.parse(this.responseText);
      $.each(data, function(index, elem) {
        create_twidl(elem._id, elem.message, elem.flag);
      });
    }
  }
  xhr.open("GET", url + "getall", true);
  xhr.send();

  $(document).on("change", ":checkbox", function() {
      if(this.checked) {
        var xhttp = new XMLHttpRequest();
        var flagID = $(this).parent().attr('id');

        xhttp.onreadystatechange = function() {
          if (this.readyState === 4 && this.status === 200) {
            console.log(flagID + " flagged!");
            $("#" + flagID).addClass("read");
          }
        }

        xhttp.open("GET", url + "flag?id=" + flagID);
        xhttp.send();
      }
  });

  $("#twibutt").click(function() {
    var xhttp = new XMLHttpRequest();
    var input = $("#twiput").val();

    xhttp.onreadystatechange = function() {
      if (this.readyState === 4 && this.status === 200) {
        console.log("ok");
        var msgID = JSON.parse(this.responseText);
        create_twidl(msgID["id"], input, false);

      }
      else if (this.status === 400) {
        Swal.fire({
          title: 'Error',
          text: 'Twidls must contain between 1 and 140 characters.',
          type: 'error'
        });
      }
      $("#twiput").val("");
    };
    xhttp.open("GET", url + "save?msg=" + input, true);
    xhttp.send();
  });
});
