var my_timer;

function timer_fun() {
    my_timer = setInterval("Get_NowTime()", 1000);
}

var timer;
var AutoRefreshFLag = "close";

function AutoRefreshTimer() {
    if (AutoRefreshFLag == "close") {
        timer = setInterval("refresh()", 2000);
        AutoRefreshFLag = "open";
        document.getElementById("autorefresh_button").value = "自动刷新";

    } else {
        timer = clearInterval(timer);
        document.getElementById("autorefresh_button").value = "手动刷新";
        AutoRefreshFLag = "close";
    }

}

//刷新函数
function refresh() {


    //从服务器获取数据
    var xmlhttp = getXMLHttpRequest();
    var url = "/cgi-bin/MyCgi2.cgi";
    var data = "refresh";
    //设置回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
            var ret = xmlhttp.responseText;

            if (ret.substr(0, 9) == "DataBack:") {

                var data = ret.substr(ret.indexOf(":") + 1);

                var Temp = data.substring(0, data.indexOf(","));

                data = data.substr(data.indexOf(",") + 1);

                var Humi = data.substring(0, data.indexOf(","));

                data = data.substr(data.indexOf(",") + 1);

                var _1_Led_1 = data.substring(0, data.indexOf(","));

                data = data.substr(data.indexOf(",") + 1);

                var _1_Led_2 = data;


                document.getElementById("Temp").innerText = Temp;
                document.getElementById("Humi").innerText = Humi;
                if (_1_Led_1 == "open") {

                    document.getElementById("1_LED:1").value = "点击关闭";

                } else {

                    document.getElementById("1_LED:1").value = "点击打开";
                }
                if (_1_Led_2 == "open") {

                    document.getElementById("1_LED:2").value = "点击关闭";

                } else {

                    document.getElementById("1_LED:2").value = "点击打开";
                }

            } else if (ret == "error") {
                alert("设备出错，请重新打开网页");
            }
        }
    }
    xmlhttp.open("POST", url, true);
    xmlhttp.send(data);
}

function Register_Button(arg) {

    if (arg == 1) //登陆 admin admin
    {
        //获取 id="usr"的数据
        var xmlhttp = getXMLHttpRequest();

        var usr = document.getElementById("usr").value;
        var pwd = document.getElementById("pwd").value;
        var data = "register:" + usr + "," + pwd;


        var url = "/cgi-bin/MyCgi.cgi";

        //设置回调函数
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
                var ret = xmlhttp.responseText;

                alert(ret);
                if (ret == "登录成功\n") {
                    window.location.href = "index1.html"
                } else if (ret == "登陆失败，用户名或密码不正确\n" || ret == "此用户未注册\n") {

                    document.getElementById("usr").value = "";
                    document.getElementById("pwd").value = "";
                }
            }
        }
        xmlhttp.open("POST", url, true);
        xmlhttp.send(data);


    } else if (arg == 2) {
        //改密码
        var id = document.getElementById("id").value;
        var oldpwd = document.getElementById("OldPwd").value;
        var newpwd = document.getElementById("NewPwd").value;
        var url = "/cgi-bin/MyCgi.cgi";
        if (id == "" || oldpwd == "" || newpwd == "") {
            alert("输入格式有误");
            return;
        }
        var xmlhttp = getXMLHttpRequest();
        var data = "Changepassword:" + id + "," + oldpwd + "," + newpwd;
        alert(data);
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
                var ret = xmlhttp.responseText;
                alert(ret);

            }
        }
        xmlhttp.open("POST", url, true);
        xmlhttp.send(data);
    }
    else if (arg == 3) {
        var xmlhttp = getXMLHttpRequest();
        var usr = document.getElementById("usr").value;
        var pwd = document.getElementById("pwd").value;
        var pwd1 = document.getElementById("pwd1").value;
        if (pwd != pwd1 || pwd == "" || usr == "" || usr.length > 15 || pwd.length > 15) {
            alert("密码和Id必须小于15个字符且不能为空，两次密码必须相同");
            return 0;
        }
        var data = "Logon:" + usr + "," + pwd;
        var url = "/cgi-bin/MyCgi.cgi";
        //设置回调函数
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
                var ret = xmlhttp.responseText;
                alert(ret);
            }
        }
        xmlhttp.open("POST", url, true);
        xmlhttp.send(data);
    }
}


//按键按下时的处理函数
function AUTO_KeyClick() {
    if (window.event.keyCode == 13) {
        if (document.all('Register') != null) {
            document.all('Register').click();
        }
    }
}
//获取当前时间
function Get_NowTime() {

    var date = new Date();

    Y = date.getFullYear() + '-';

    M = (date.getMonth() + 1 < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1) + '-';

    D = date.getDate() + ' ';

    h = date.getHours() + ':';

    m = date.getMinutes() + ':';

    s = date.getSeconds();
    document.getElementById("date").innerHTML = Y + M + D + h + m + s;
    if (document.getElementById("date_id") != null)
        document.getElementById("date_id").value = Y + M + D + h + m + s;
}




$(function () {
    /*注意这里的鼠标事件，只给父菜单定义了一个mouseover，用来触发子菜单的显示，但不添加mouseout事件，否则鼠标还没移到子菜单上，它就消失了。*/

    $(".nav-portrait").mouseover(function () {
        $(".nav-second-ul").css('display', 'block');
    });

    $(".nav-second-ul").mouseover(function () {
        $(".nav-second-ul").css('display', 'block');
    });

    $(".nav-second-ul").mouseout(function () {
        $(".nav-second-ul").css('display', 'none');
    });
})




function getXMLHttpRequest() {
    var xmlhttp = null;
    if (window.XMLHttpRequest) //自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    { // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest(); //创建请求对象
    } else ////如果浏览器是底版本的
    { // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); //创建请求对象
    }
    return xmlhttp; //返回请求对象
}




function Ledcrtl(arg) {

    //创建xmlHttpRequest对象
    //arg == 0 为第一个设备的LED1
    //arg == 1 为第一个设备的LED2
    //arg == 2 为第二个设备的LED1
    //arg == 3 为第二个设备的LED2
    var xmlhttp = getXMLHttpRequest();
    var url = "/cgi-bin/MyCgi2.cgi";
    var data = "";
    var tmp = document.getElementById(arg).value;


    if (tmp == "点击打开") {
        data = arg + "open";
    }
    else if (tmp == "点击关闭") {
        data = arg + "close";
    }



    //设置回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
            var ret = xmlhttp.responseText;

            if (ret == "OK") {
                var tmp = document.getElementById(arg).value;
                if (tmp == "点击打开") {
                    document.getElementById(arg).value = "点击关闭";
                } else {
                    document.getElementById(arg).value = "点击打开";
                }
            }
        }
    }
    //open
    xmlhttp.open("POST", url, true);
    //send
    xmlhttp.send(data);


}


function JUMP(arg) {
    
    document.getElementById("one").innerHTML = "";
    document.getElementById('one').setAttribute('class', 'MyType');
    switch (arg) {
        
        case 0:

            var a = '<input type="button" class="active" value="注销" onclick="cancellation()" />\
                        <div class="container">\
                            <p class="title">\
                                <input type="button" class="data1" value="历史数据" onclick="HistoricalRecord()" />\
                                <input type="button" class="data2" id="date_id" value="        时间          " />\
                                <label id="device_id">1号棚</label>\
                                <input type="button" class="active" value="手动刷新" id="autorefresh_button" onclick="AutoRefreshTimer()" />\
                                <input type="button" class="active" value="刷新" onclick="refresh()" />\
                            </p>\
                            <div class="main">\
                                <div class="left">\
                                    <h6>\
                                        <a>最小值</a>\
                                        <a>最大值</a>\
                                    </h6>\
                                    <p class="wrap">\
                                        <input value="24" id="value_c_min" />\
                                        <input value="24" id="value_c_max" />℃\
                                    </p>\
                                    <p class="wrap">\
                                        <input value="24" id="value_h_min" />\
                                        <input value="24" id="value_h_max" />%\
                                    </p>\
                                    <br>\
                                    </div>\
                                <div class="middle">\
                                    <h6>\
                                        <a>采集节点</a>\
                                        <a>当前值</a>\
                                    </h6>\
                                    <p>\
                                        <a>温度：</a>\
                                        <th><span id="Temp">10</span></th>\
                                        <label>℃</label>\
                                    </p>\
                                    <br>\
                                    <p>\
                                        <a>湿度：</a>\
                                        <th><span id="Humi">30</span></th>\
                                        <label>%</label>\
                                        </p>\
                                    <br>\
                                    <p>\
                                    </p>\
                                    <br>\
                                    <br>\
                                    <br>\
                                    <br>\
                                    <br>\
                                    <th align="center">\
                                        <input type="button" class="active_1" value="保存配置" onclick="on_button_seve()" />\
                                    </th>\
                                </div>\
                                <div class="right">\
                                    <h6>\
                                        <a>控制节点</a>\
                                        <a>状态</a>\
                                        <a> 手动状态</a>\
                                    </h6>\
                                    <p>\
                                        <a>LED_1：</a>\
                                        <th><input type="button"  value="点击打开" id="1_LED:1" onclick="Ledcrtl(\'1_LED:1\')"></th>\
                                        <th></th>\
                                        <th>\
                                            <select id="select_query_t" onchange="on_select1()">\
                                                    <option value="auto_t">自动</option>\
                                                    <option value="cur_t">手动</option>\
                                                    </select>\
                                        </th>\
                                    </p>\
                                    <br>\
                                    <p>\
                                        <a>LED_2：</a>\
                                        <th><input type="button"  value="点击打开" id="1_LED:2" onclick="Ledcrtl(\'1_LED:2\')"></th>\
                                        <th></th>\
                                        <th>\
                                            <select id="select_query_h" onchange="on_select2()">\
                                                <option value="auto_h">自动</option>\
                                                <option value="cur_h">手动</option>\
                                                </select>\
                                        </th>\
                                    </p>\
                                    <br>\
                                    ';
            document.getElementById("one").innerHTML = a;
            break;
        case 1:

            break;
        case 2:
            var a = '<p style="position:relative;top: 150px;"><br><label onclick="Jump(2);">想的美，没有帮助，自立更生</label></p>';
            document.getElementById("one").innerHTML = a;

            break;
        case 3:
            var a = '<p id="about" style="position:relative;top: 150px;"><br><label id="lab">这个路由器，可快了，从来不会断网</label></p>';
            document.getElementById("one").innerHTML = a;
            Jump(3);
            break;
        case 4:
            var a = '\
            <div>\
            <br>\
            <br>\
            <label>ID</label>\
            <input placeholder="请输入ID" id="id" />\
            <br>\
            <br>\
            <label>旧密码</label>\
            <input placeholder="请输入旧密码" id="OldPwd" />\
            <br>\
            <br>\
            <label>新密码</label>\
            <input placeholder="请输入新密码" id="NewPwd" />\
            <br>\
            <br>\
            <input type="button" style="color: black; top: 40px;right: 20px; "value="确认更改" onclick="Register_Button(2) "></input>\
            </div>';
            document.getElementById("one").innerHTML = a;
            var temp = document.getElementById('one').removeAttribute('class');
            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 100:
            var a = '<h3>欢迎你</h3>\
            <form action="#" name="f" method="post">\
            <div class="input_outer">\
                <span class="u_user"></span>\
                <input name="logname" class="text" style="color: #FFFFFF !important" type="text" id="usr" placeholder="请输入账户">\
            </div>\
            <div class="input_outer">\
                <span class="us_uer"></span>\
                <input name="logpass" class="text" style="color: #FFFFFF !important; position:absolute; z-index:100;" value="" id="pwd" type="password" placeholder="请输入密码">\
            </div>\
            <div class="mb2"><a class="act-but submit" onclick="Register_Button(1)" id="Register" style="color: #FFFFFF">登录</a></div>\
            <input type="button" onclick="JUMP(101)" id="Register1" value="注册" >\
        </form>';
            document.getElementById("BOX").innerHTML = a;
            break;
        case 101:
            var a = '<h3>欢迎你</h3>\
            <form action="#" name="f" method="post">\
            <div class="input_outer">\
                <span class="u_user"></span>\
                <input name="logname" class="text" style="color: #FFFFFF !important" type="text" id="usr" placeholder="请输入账户">\
            </div>\
            <div class="input_outer">\
                <span class="us_uer"></span>\
                <input name="logpass" class="text" style="color: #FFFFFF !important; position:absolute; z-index:100;" value="" id="pwd" type="password" placeholder="请输入密码">\
            </div>\
            <div class="input_outer">\
                <span class="us_uer"></span>\
                <input name="logpass1" class="text" style="color: #FFFFFF !important; position:absolute; z-index:100;" value="" id="pwd1" type="password" placeholder="请再次输入密码">\
            </div>\
            <div class="mb2"><a class="act-but submit" onclick="Register_Button(3)" id="Register" style="color: #FFFFFF">注册</a></div>\
            <input type="button" onclick="JUMP(100)" id="Register1" value="返回" >\
        </form>';
            document.getElementById("BOX").innerHTML = a;
            break;
    }

}
/**************关于的文字炫彩效果*************************** */
var amy_timer;

function Jump(arg) {
    if (arg == 3)
        amy_timer = setInterval("my_fun()", 100);
    else if (arg == 2)
        window.location = "http://www.baidu.com";
}

function my_fun() {
    var div = document.getElementById("lab");
    div.style.color = div.style.color == 'red' ?
        'green' : (div.style.color == 'green' ? 'blue' : 'red');
}
/**************结束**************************************** */
function HistoricalRecord()
{alert("wo dima l ");
    var xmlhttp = getXMLHttpRequest()
    var url = "/cgi-bin/MyCgi2.cgi";
    var data = "GETHistoricalRecord";

    //设置回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
            var ret = xmlhttp.responseText;
            alert(ret);
            
        }
    }
    xmlhttp.open("POST", url, true);
    xmlhttp.send(data);


}
