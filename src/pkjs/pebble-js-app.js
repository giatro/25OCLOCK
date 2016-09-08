Pebble.addEventListener('ready', function() {
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://giatro.me/25OCLOCK/conf/index.html';
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var backgroundColor = configData['background_color'];
  var firstForegroundColor = configData['first_foreground_color'];
  var secondForegroundColor = configData['second_foreground_color'];
  var random = configData['random_checkbox'];
  var fontIndex = configData['font_index'];
  var layoutRow0 = configData['layout_row_0'];
  var layoutRow1 = configData['layout_row_1'];
  var layoutRow2 = configData['layout_row_2'];
  var layoutRow3 = configData['layout_row_3'];
  var layoutRow4 = configData['layout_row_4'];
    
  var dict = {};
  dict['KEY_BG_COLOR_RED'] = parseInt(backgroundColor.substring(2, 4), 16);
  dict['KEY_BG_COLOR_GREEN'] = parseInt(backgroundColor.substring(4, 6), 16);
  dict['KEY_BG_COLOR_BLUE'] = parseInt(backgroundColor.substring(6), 16);
  dict['KEY_ST_COLOR_RED'] = parseInt(firstForegroundColor.substring(2, 4), 16);
  dict['KEY_ST_COLOR_GREEN'] = parseInt(firstForegroundColor.substring(4, 6), 16);
  dict['KEY_ST_COLOR_BLUE'] = parseInt(firstForegroundColor.substring(6), 16);
  dict['KEY_ND_COLOR_RED'] = parseInt(secondForegroundColor.substring(2, 4), 16);
  dict['KEY_ND_COLOR_GREEN'] = parseInt(secondForegroundColor.substring(4, 6), 16);
  dict['KEY_ND_COLOR_BLUE'] = parseInt(secondForegroundColor.substring(6), 16);
  dict['KEY_RANDOM'] = random ? 1 : 0;
  dict['KEY_FONT_INDEX'] = parseInt(fontIndex);
  dict['KEY_LAYOUT_ROW_0'] = parseInt(layoutRow0);
  dict['KEY_LAYOUT_ROW_1'] = parseInt(layoutRow1);
  dict['KEY_LAYOUT_ROW_2'] = parseInt(layoutRow2);
  dict['KEY_LAYOUT_ROW_3'] = parseInt(layoutRow3);
  dict['KEY_LAYOUT_ROW_4'] = parseInt(layoutRow4);
  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});


