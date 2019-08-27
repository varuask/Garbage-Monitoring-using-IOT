<script>
		window.onload = function() {
            loaddata();
        };
		function loaddata(){
				var url = "https://monker.000webhostapp.com/api/weather/read_all.php";
				$.getJSON(url, function(data) {
                    var val= data;
                  var humid=(data['weather'][(Object.keys(data['weather']).length)-1]['hum']);
                   var temper=(data['weather'][(Object.keys(data['weather']).length)-1]['temp']);
                   document.getElementById("temperature").innerHTML = "<img src = 'temperature.png' height=\"60px\" width=\"60px\" style='vertical-align: middle' />  " +temper;
                   document.getElementById("humidity").innerHTML = "<img src = 'humidity.png' height=\"60px\" width=\"60px\" style='vertical-align: middle' />  "+humid;
					console.log(data['weather'][(Object.keys(data['weather']).length)-1]['temp']);
				});

        }
        window.setInterval(function(){
        loaddata();
            }, 5000);

	</script>
