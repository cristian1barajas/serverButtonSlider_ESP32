function rangeSlide(value) {
    var sliderValue = document.getElementById("pwmSlider").value;
    document.getElementById('rangeValue').innerHTML = sliderValue;
    console.log(sliderValue);
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/slider?value=" + sliderValue, true);
    xhr.send();
}

function rangeSlide2(value) {
    var sliderValue2 = document.getElementById("pwmSlider2").value;
    document.getElementById('rangeValue2').innerHTML = sliderValue2;
    console.log(sliderValue2);
    var xhr2 = new XMLHttpRequest();
    xhr2.open("GET", "/slider2?value=" + sliderValue2, true);
    xhr2.send();
}

function toogleButtonOn() {
    document.getElementById('gpioValue').innerHTML = "ON";
    console.log("Ejecuta1");
    var xhr3 = new XMLHttpRequest();
    xhr3.open("GET", "/update?value=1", true);
    xhr3.send();
}

function toogleButtonOff() {
    document.getElementById('gpioValue').innerHTML = "OFF";
    console.log("Ejecuta2");
    var xhr4 = new XMLHttpRequest();
    xhr4.open("GET", "/update?value=0", true);
    xhr4.send();
}