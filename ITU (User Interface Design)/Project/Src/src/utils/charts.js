/**
 * Project:     VUT_ITU_Project
 * File name:   fakeApi.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 * Description:
 */

import Moment from 'moment';
import { Data } from 'victory';
import {toFahrenheit} from './utils';

const labelSpace = 60;

export function filterChartData(histData, from, to) {
    var res = [];
    histData.forEach((el, i) => {
        var date = Date.parse(el.date);
        if (date >= from && date <= to)
        {
            let tmp = { ...el,"date": date};
            res.push(tmp);
        }
    });

    return res;
}

export function calcDateRange(data, range) {
    var startDate = new Date();
    var tickFormat;

    switch (range) {
        case "all":
            startDate = new Date(0);
            tickFormat = (t) => `${Moment(new Date(t)).format('YYYY')}`;
            break;
        case "year":
            startDate.setFullYear(new Date().getFullYear());
            startDate.setMonth(0);
            startDate.setDate(1);
            tickFormat = (t) => `${Moment(new Date(t)).format('MMM')}`;
            break;
        case "month":
            startDate.setMonth(new Date().getMonth());
            startDate.setDate(1);
            tickFormat = (t) => `${Moment(new Date(t)).format('D. M.')}`;
            break;
        case "week":
            startDate.setDate(new Date().getDate() - 6);
            tickFormat = (t) => `${Moment(new Date(t)).format('ddd')}`;
            break;
        case "day":
            startDate.setDate(new Date().getDate());
            tickFormat = (t) => `${Moment(new Date(t)).format('hh:mm')}`;
            break;
        default:
    }

    startDate.setHours(0);
    startDate.setMinutes(0);

    return {
        dateRange: range,
        data: filterChartData(data, startDate, new Date()),
        tickFormat: tickFormat,
    };
}
export function calcDateRangeSlider(data, range, count) {
    var startDate = new Date();
    var tickFormat;
/**/
    switch (range) {
        case "all":
            startDate = new Date(0);
            tickFormat = (t) => `${Moment(new Date(t)).format('YYYY')}`;
            break;
        case "year":
            startDate.setFullYear(new Date().getFullYear());
            startDate.setMonth(0);
            startDate.setDate(1);
            tickFormat = (t) => `${Moment(new Date(t)).format('MMM')}`;
            break;
        case "months":
                startDate.setMonth(new Date().getMonth() - count);
                startDate.setDate(1);
                tickFormat = (t) => `${Moment(new Date(t)).format('MMM')}`;
            break;
        case "month":
            startDate.setDate(new Date().getDate() - 7*(4));

            tickFormat = (t) => `${Moment(new Date(t)).format('D. M.')}`;

            break;
        case "weeks":
            startDate.setDate(new Date().getDate() - 7*(count));
            tickFormat = (t) => `${Moment(new Date(t)).format('D.M.')}`;
            
            break;
 
        case "week":
            startDate.setDate(new Date().getDate() - 7*(count));
            tickFormat = (t) => `${Moment(new Date(t)).format('ddd')}`;
            break;
        case "days":
            startDate.setDate(new Date().getDate() - count);
            tickFormat = (t) => `${Moment(new Date(t)).format('ddd')}`;
            break;
        case "day":
            startDate.setDate(new Date().getDate()-1);
            tickFormat = (t) => `${Moment(new Date(t)).format('hh:mm.')}`;
            
            break;
        default:
    }

    startDate.setHours(0);
    startDate.setMinutes(0);

    return {
        dateRange: range,
        data: filterChartData(data, startDate, new Date()),
        tickFormat: tickFormat,
    };
}
export function parseToChartData(histData, prop, screenWidth, isFahrenheit) {
    var res = [];
    var cnt = histData.length;
    var sel = cnt / (screenWidth / labelSpace);

    if (sel < 1)
        sel = 1;
    sel = Math.round(sel);

    histData.forEach((el, i) => {

        if (i % sel === 0)
        {
            var tmp = {
                x: new Date(el.date),
                y: 0,
                show: false
            }

            if (prop === "temp") {
                tmp.y = isFahrenheit ? toFahrenheit(el.temp) : el.temp;
            } else if (prop === "humi") {
                tmp.y = el.humi;
            }

            // Manage labeling
            if (i > 0 && i < cnt - sel)
                tmp.show = true;

            res.push(tmp);
        }
    });

    return res;
}