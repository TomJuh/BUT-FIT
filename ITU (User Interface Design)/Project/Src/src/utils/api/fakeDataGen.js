/**
 * Project:     VUT_ITU_Project
 * File name:   fakeDataGen.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 * Description:
 */

const histCnt = 1095;

const tempMax = 35.0;
const tempMin = 30.0;

const humiMax = 50.0;
const humiMin = 60.0;

const dateEnd       = new Date();
const dateDecrement = new Date(28800000);

function round(nmb, decPlaces)
{
  return (nmb * 10^decPlaces) / (10^decPlaces); 
}

function toRange(nmb, min, max)
{
  return ((nmb - min) % (max - min)) + min;
}

function getSensorHist() {

  let hist = [];
  let temp = (tempMax / 2) * (Math.random());
  let humi = (humiMax / 2) * (Math.random());

  for (var i = 0; i < histCnt; i++)
  {
    var histData = {
      "temp": toRange(temp, tempMin, tempMax),
      "humi": toRange(humi, humiMin, humiMax),
      "batt": 99,
      "date": (new Date(dateEnd - dateDecrement * i)).toJSON(),
    };

    hist.push(histData);
    temp = temp + 20*(Math.random() - 0.4);
    humi = humi + 20*(Math.random() - 0.5);
  }

  return hist;
}

const getSensorData = function (sensor)
{
    var histData = getSensorHist();

    return {
        ...sensor,
        currData: { ...histData[0] },
        histData: histData,
    };
}

export default getSensorData;