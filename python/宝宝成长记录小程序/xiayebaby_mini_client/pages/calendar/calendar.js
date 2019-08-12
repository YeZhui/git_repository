// pages/calendar/calendar.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    day: new Date().getDate(),
    daysStyle: [
      { month: 'current', day: new Date().getDate(), color: 'white', background: '#eb4986' },
      { month: 'current', day: new Date().getDate(), color: 'white', background: '#eb4986' }
    ],
    selectYear: new Date().getFullYear(),  //选中年份默认当前
    selectMonth: new Date().getMonth() + 1, //默认选中当前月
    selectDay: new Date().getDate(),
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    //const days_count = new Date(this.data.year, this.data.month, 0).getDate();
    /*
    let daysStyle = new Array;
    daysStyle.push({
      month: 'current', day: this.data.day, color: 'white', background: '#eb4986'
    });
    this.setData({
      daysStyle
    });
    */
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

  },

  //日期点击事件
  dayClick: function(e) {
    this.data.selectYear = e.detail.year;
    this.data.selectMonth = e.detail.month;
    this.data.selectDay = e.detail.day;
    let changeDay = `daysStyle[1].day`;
    let changeBg = `daysStyle[1].background`;
    this.setData({
      [changeDay]: this.data.selectDay,
      [changeBg]: '#B452CD'
    })
  },
  //添加纪念日
  addCommDay: function(e) {
    wx.navigateTo({
      url: '../addCommDay/addCommDay?year=' + this.data.selectYear + '&month=' + this.data.selectMonth + '&day=' + this.data.selectDay,  //?key=value&key2=value2
    })
  },
  showCommDay: function(e) {
    wx.navigateTo({
      url: '../showCommDay/showCommDay',
    })
  }
})