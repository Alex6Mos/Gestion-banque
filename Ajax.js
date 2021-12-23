var connect = $('#conecte');
var form = $('#conection');

form.submit(function (e) { 
    //e.preventDefault();
    $.ajax({
        type: "session",
        url: "AjaxConnect.php",
        data: {click: 1},
        success: function (response) 
        {
            if(response === "1")
            {
                alert(response);
                form.submit();
            }
            else
            {
                alert("nom");
            }
        }
    });
    
});
