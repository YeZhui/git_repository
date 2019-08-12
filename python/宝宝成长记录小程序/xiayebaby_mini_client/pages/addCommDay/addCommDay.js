// pages/addCommDay/addCommDay.js
var appInstance = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    date:'',
    commDays: ["开始备孕", "怀孕第一天", "怀孕第三周", "怀孕第六周", "怀孕第九周", "预产期", "出生日", "出生一个月"],
    commDaysIndex: 0,
    commDayStr:'',
  },

  bindDateChange: function (e) {
    this.setData({
      date: e.detail.value
    })
    console.log('选择日期：' + this.data.date)
  },
  bindCommDayChange: function (e) {
    this.setData({
      commDaysIndex: e.detail.value,
      commDayStr: this.data.commDays[e.detail.value]
    })
    console.log('纪念日：' + this.data.commDayStr)
  },
  bindCommName (e) {
    this.setData({
      commDaysIndex: 99,//若是手动输入，默认下标99
      commDayStr: e.detail.value
    })
    console.log('手动输入纪念日:' + this.data.commDayStr)
  },
  addCommDay: function (e) {
    //点击确定按钮调用
    wx.request({
      url: 'http://192.168.233.250:8000/add_comm_day', 
      data: {
        date: this.data.date,
        commDayFlag: this.data.commDaysIndex,
        commDayStr: this.data.commDayStr,
        custId: appInstance.globalData.openid,
      },
      success (res) {
        console.log(res.data)
      },
      fail (res) {

      }
    })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let date = '';
    date = options.year + '-' + options.month + '-' + options.day;
    this.setData({
      date
    });
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})