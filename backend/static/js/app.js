
$("#online_div").hide();
$("#offline_div").show();

setInterval(function() {
    $.ajax({
        type: "POST",
        url: '/',
        data: JSON.stringify({"command": "status"}),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        success: function (result) {
            if (result['status'] === 'OFFLINE') {
                $('#offline_div').show();
                $('#online_div').hide();
            } else {
                $('#online_div').show();
                $('#offline_div').hide();
            }
            $('#act').text(result['act'])
        }
    })
}, 1000);

$("#switch_on").on("click", function() {
    $.ajax({
        type: "POST",
        url: '/',
        data: JSON.stringify({"command": "esp8266_power_on"}),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        success: function (result) {
        }
    })
});

$("#switch_off").on("click", function() {
    $.ajax({
        type: "POST",
        url: '/',
        data: JSON.stringify({"command": "esp8266_power_on_reset"}),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        success: function (result) {
        }
    })
});
