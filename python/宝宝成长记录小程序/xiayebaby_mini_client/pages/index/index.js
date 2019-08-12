// 引用百度地图微信小程序JSAPI模块 
//var bmap = require('../../libs/bmap-wx.js');
//const app = getApp()
var appInstance = getApp()
Page({
  data: {
    date: '',
    selectYear: new Date().getFullYear(),  //选中年份默认当前
    selectMonth: new Date().getMonth() + 1, //默认选中当前月
    selectDay: new Date().getDate(),
    commDays: ["晴", "阴", "多云", "小雨", "中雨", "大雨", "暴雨", "打雷"],
    commDaysIndex: 0,
    readOnly: false,
    placeholder: '开始输入...',
    address: '',
    state: '',
    thinkText:'',  //用于保存今日想法
    thinkHtml:'', //用于保存今日想法html
    todayMessage:'', //用于保存今日寄语

  },
  bindDateChange: function (e) {
    this.setData({
      date: e.detail.value
    })
    console.log('选择日期：' + this.data.date)
  },
  bindCommDayChange: function (e) {
    this.setData({
      commDaysIndex: e.detail.value
    })
    console.log('天气：' + this.data.commDays[this.data.commDaysIndex])
  },
  bindAddressInput: function (e) {
    this.setData({
      address: e.detail.value
    })
    console.log('地点:' + this.data.address)
  },
  bindStatusInput: function (e) {
    this.setData({
      state: e.detail.value
    })
    console.log('孕期状态:' + this.data.state)
  },
  addDiary() {
    //点击确定按钮调用
    console.log('想法保存成功!')
    wx.request({
      url: 'http://192.168.233.250:8000/add_diary', 
      data: {
        custId: appInstance.globalData.openid,
        moodDate: this.data.date,
        moodWeather: this.data.commDays[this.data.commDaysIndex],  //天气
        moodAddress: this.data.address,
        moodState: this.data.state,
        content: this.data.thinkText,
        todayMessage: this.data.todayMessage,
        
      },
      success (res) {
        console.log(res.data)
      },
      fail (res) {

      }
    })
  },
  readOnlyChange() {
    this.setData({
      readOnly: !this.data.readOnly
    })
  },
  onLoad: function () {
    var that = this;
    let date = '';
    date = that.data.selectYear + '-' + that.data.selectMonth + '-' + that.data.selectDay;
    that.setData({
      date
    })
    this.canUse = true
    wx.loadFontFace ({
      family: 'Pacifico',
      source: 'url("https://sungd.github.io/Pacifico.ttf")',
      success: console.log
    })
    // 新建百度地图对象 
    /*
    var BMap = new bmap.BMapWX({
      ak: 'A1vHf02Ma9puCnmG2XZ2Q1L7qbjBjzVf'
    });
    var fail = function (data) {
      console.log(data)
    };
    var success = function (data) {
      var weatherData = data.currentWeather[0];
      weatherData = weatherData.currentCity + '\n' + weatherData.date + '\n' + weatherData.weatherDesc + ', ' + weatherData.wind + '\n' + weatherData.temperature;
      that.setData({
        weatherData: weatherData
      });
    }
    // 发起weather请求 
    BMap.weather({
      fail: fail,
      success: success
    });
    */
  },
  onEditorReady() {
    const that = this
    wx.createSelectorQuery().select('#editor').context(function (res) {
      that.editorCtx = res.context
    }).exec()
  },
  undo() {
    this.editorCtx.undo()
  },
  redo() {
    this.editorCtx.redo()
  },
  format(e) {
    if ( !this.canUse ) return
    const { name, value } = e.target.dataset
    if (!name) return
    // console.log('format', name, value)
    this.editorCtx.format(name, value)
  },
  onStatusChange(e) {
    const formats = e.detail
    this.setData({formats})
  },
  onLostFocus(e) {
    //今日想法保存
    this.setdata({
      thinkText: e.detail.text,
      thinkHtml: e.detail.html
    })
  },
  onTodaySayInput(e) {
    //今日想法保存
    this.setdata({
      todayMessage: e.detail.value
    })
  },
  insertDivider() {
    this.editorCtx.insertDivider({
      success() {
        console.log('insert divider success')
      }
    })
  },
  clear() {
    this.editorCtx.clear({
      success() {
        console.log('clear success')
      }
    })
  },
  removeFormat() {
    this.editorCtx.removeFormat()
  },
  insertDate() {
    const date = new Date()
    const formatDate = `${date.getFullYear()}/${date.getMonth() + 1}/${date.getDate()}`
    this.editorCtx.insertText({
      text: formatDate
    })
  },
  insertImage() {
    const that = this
    wx.chooseImage({
      count: 1,
      success() {
        that.editorCtx.insertImage({
          src: 'https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1543767268337&di=5a3bbfaeb30149b2afd33a3c7aaa4ead&imgtype=0&src=http%3A%2F%2Fimg02.tooopen.com%2Fimages%2F20151031%2Ftooopen_sy_147004931368.jpg',
          data: {
            id: 'abcd',
            role: 'god'
          },
          success() {
            console.log('insert image success')
          }
        })
      }
    })
  }
})