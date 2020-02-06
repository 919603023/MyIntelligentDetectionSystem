var data = "";
var close1_open = false;
var close1_state = false;

function on_button_close1() {
    if (close1_open)
        if (!close1_state) {
            //close1_state = true;
            //document.getElementById("close_1").value = "开启";
            data = "modify:pubdemo_1,0,0,0,0,4,0";
            deal_request();
        } else {
            //close1_state = false;
            // document.getElementById("close_1").value = "关闭";
            data = "modify:pubdemo_1,0,0,0,0,3,0";
            deal_request();
        }
}
var close2_open = false;
var close2_state = false;

function on_button_close2() {
    if (close2_open)
        if (!close2_state) {
            //close2_state = true;
            // document.getElementById("close_2").value = "开启";
            data = "modify:pubdemo_1,0,0,0,0,0,4";
            deal_request();
        } else {
            //close2_state = false;
            // document.getElementById("close_2").value = "关闭";
            data = "modify:pubdemo_1,0,0,0,0,0,3";
            deal_request();
        }
}



function get_xmlreq() {
    var xmlhttp = null;
    if (window.XMLHttpRequest) {
        xmlhttp = new XMLHttpRequest();
    } else {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    return xmlhttp;
}

var sta = 1;

function deal_request() {
    var xmlhttp = get_xmlreq();
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            var ret = xmlhttp.responseText;
            //alert(ret);
            //refresh,2019. 9.31 14:42:32,pubdemo_1,26,20,0,0,pubdemo_1,100,0,100,0,0,1,
            var buf = ret.split(",");
            //for(var i = 0;i < 13;i++)
            //alert(buf[i]);
            //alert(buf[0]);
            if (buf[0] == "refresh") {
                document.getElementById("value_c").innerHTML = buf[3]; //温度
                document.getElementById("value_h").innerHTML = buf[4]; //湿度
                //document.getElementById("value_i").innerHTML = null;//光照
                document.getElementById("date_id").value = buf[1]; //时间
                if (buf[5] == 0) {
                    close1_state = false;
                    document.getElementById("close_1").value = "关闭";
                } else if (buf[5] == 1) {
                    close1_state = true;
                    document.getElementById("close_1").value = "开启";
                }
                if (buf[6] == 0) {
                    close2_state = false;
                    document.getElementById("close_2").value = "关闭";
                } else if (buf[6] == 1) {
                    close2_state = true;
                    document.getElementById("close_2").value = "开启";
                }
                //alert(sta);
                if (sta) { //数值范围
                    document.getElementById("value_c_min").value = buf[9];
                    document.getElementById("value_c_max").value = buf[8];
                    document.getElementById("value_h_min").value = buf[11];
                    document.getElementById("value_h_max").value = buf[10];

                    //document.getElementById("value_i_max").value = null;
                    document.getElementById("device_id").innerHTML = buf[2];
                    if (buf[12] == 0) {
                        document.getElementById("select_query_t").value = "auto_t";
                        close1_open = false;
                    } else if (buf[12] == 1) {
                        document.getElementById("select_query_t").value = "cur_t";
                        close1_open = true;
                    }
                    if (buf[13] == 0) {
                        document.getElementById("select_query_h").value = "auto_h";
                        close2_open = false;
                    } else if (buf[13] == 1) {
                        document.getElementById("select_query_h").value = "cur_h";
                        close2_open = true;
                    }
                }
            } else if (buf[0] == "modify") {
                alert(buf[1]);
            } else
                alert(ret);



        }
    }

    var myurl = "/cgi-bin/Arg_data.cgi?";
    myurl += data;
    //alert(myurl);
    xmlhttp.open("GET", myurl, true);
    xmlhttp.setRequestHeader("If-Modified-Since", "0");
    xmlhttp.send();
}

function init_getdata() {

    //on_button_show();
    data = "refresh:pubdemo_1";
    //alert(data);
    sta = 1;
    deal_request();
}

function clock() {
    //var d=new Date();
    //var t=d.toLocaleTimeString();
    sta = 0;
    data = "refresh:pubdemo_1";
    //alert(data);
    deal_request();
    if (colck_state)
        setTimeout("clock()", 1000);
}

function on_button_show(arg) {
    sta = 1;
    alert(arg);
    data = "refresh:pubdemo_1";
    //alert(data);
    deal_request();
}

var colck_state = false;

function on_button_auto() {
    if (!colck_state) {
        colck_state = true;
        document.getElementById("button_1").value = "自刷新";
        document.getElementById("Refresh_auto").innerHTML = "自刷新";
        setTimeout("clock()", 1000);
    } else {
        colck_state = false;
        document.getElementById("button_1").value = "无刷新";
        document.getElementById("Refresh_auto").innerHTML = "无刷新";
    }

}

function on_button_seve() {
    //modify:'pubdemo_1',50,20,50,20,1,10
    data = "modify:";
    data += document.getElementById("device_id").innerHTML;
    data += ",";
    data += document.getElementById("value_c_max").value;
    data += ",";
    data += document.getElementById("value_c_min").value;
    data += ",";
    data += document.getElementById("value_h_max").value;
    data += ",";
    data += document.getElementById("value_h_min").value;
    data += ",";
    if (document.getElementById("select_query_t").value == "cur_t") {
        data += "1";
        close1_open = true;
    } else if (document.getElementById("select_query_t").value == "auto_t") {
        data += "0";
        close1_open = false;
    }
    data += ",";
    if (document.getElementById("select_query_h").value == "cur_h") {
        data += "1";
        close2_open = true;
    } else if (document.getElementById("select_query_h").value == "auto_h") {
        data += "0";
        close2_open = false;
    }
    //alert(data);
    deal_request();
}

function cancellation() { //返回登录界面
    window.location.href = "http://10.0.112.114/index.html";
}

function historical_data() { //查看历史数据
    window.location.href = "http://10.0.112.114/highcharts.html"; //"http://10.0.112.114/historical_data.html";
}