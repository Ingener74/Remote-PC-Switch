
$("#online_div").hide();
$("#offline_div").show();

setInterval(function() {
    $.ajax({
        url: '/esp8266_status',
        success: function (result) {
            if (result['status'] === 'OFFLINE') {
                $('#offline_div').show();
                $('#online_div').hide();
            } else {
                $('#online_div').show();
                $('#offline_div').hide();
            }
        }
    });
}, 1000);

$("#switch_on").on("click", function() {
    $.ajax({
        url: '/esp8266_poweron',
        success: function (result) {
        }
    });
});

$("#switch_off").on("click", function() {
    $.ajax({
        url: '/esp8266_poweroff',
        success: function (result) {
        }
    });
});

console.log("Done");

$("#switch_off").on("click", function () {

});