
// read data from SD card
myFile = SD.open(file_name, FILE_READ);
if (myFile) {
  int i = 0;
  char data[100];
  while (myFile.available()) {
    char c = myFile.read();
    if (c == '\n') {
      data[i] = '\0';
      char *tok = strtok(data, ",");
      int timestamp = atoi(tok);
      for (int j = 0; j < SENSOR_NUM; j++) {
        tok = strtok(NULL, ",");
        float humidity = atof(tok);
        tok = strtok(NULL, ",");
        float temperature = atof(tok);
        // update the corresponding graph data
        // assuming graph_data is a 2D array of floats
        graph_data[j*2][timestamp] = humidity;
        graph_data[j*2+1][timestamp] = temperature;
      }
      i = 0;
    } else {
      data[i] = c;
      i++;
    }
  }
  myFile.close();
}

// create an array of timestamps for x-axis
float timestamps[SENSOR_DATA_LEN];
for (int i = 0; i < SENSOR_DATA_LEN; i++) {
  timestamps[i] = i*measure_interval;
}

// send the HTML response with the graphs
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");
client.println("Refresh: 5");
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<head><title>Graphs</title><script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script></head>");
client.println("<body>");
client.println(formatString(buf,"<h3>Time : %d/%d/%d %d:%d:%d</h3>",year(epoch), month(epoch), day(epoch), hour(epoch), minute(epoch), second(epoch)));

for (int i = 0; i < SENSOR_NUM; i++) {
  // create an array of humidity data for the current sensor
  float humidity_data[SENSOR_DATA_LEN];
  for (int j = 0; j < SENSOR_DATA_LEN; j++) {
    humidity_data[j] = graph_data[i*2][j];
  }

  // create an array of temperature data for the current sensor
  float temp_data[SENSOR_DATA_LEN];
  for (int j = 0; j < SENSOR_DATA_LEN; j++) {
    temp_data[j] = graph_data[i*2+1][j];
  }

  // create trace for humidity data
  char trace_humidity[200];
  sprintf(trace_humidity, "{x: [%d], y: [%f], mode: 'lines', name: 'Humidity Sensor %d'}", (int)timestamps[0], humidity_data[0], i+1);

  // create trace for temperature data
  char trace_temp[200];
  sprintf(trace_temp, "{x: [%d], y: [%f], mode: 'lines', name: 'Temperature Sensor %d', yaxis: 'y2'}", (int)timestamps[0], temp_data[0], i+1);

  // create layout for the plot
  char layout[300];
  sprintf(layout, "{title: 'Sensor %d', xaxis: {title: 'Time (s)'}, yaxis: {title: 'Humidity (%rh)'}, yaxis2: {title: 'Temperature (°C)', overlaying: 'y', side: 'right'}}", i+1);

  // create div and script for the plot
  char div_id[10];
  sprintf(div_id, "sensor%d", i+1);
  client.println(formatString(buf, "<h2>Sensor %d</h2><div id=\"%s\"></div>", i+1, div_id));
  client.println("<script>");
  client.println(formatString(buf, "var data%d = [%s, %s];", i, trace_humidity, trace_temp));
  client.println(formatString(buf, "var layout%d = %s;", i, layout));
  client.println(formatString(buf, "Plotly.newPlot('%s', data%d, layout%d);", div_id, i, i));
  for (int j = 1; j < SENSOR_DATA_LEN; j++) {
    client.println(formatString(buf, "Plotly.extendTraces('%s', {x: [[%d]], y: [[%f], [%f]]}, [0, 1]);", div_id, (int)timestamps[j], humidity_data[j], temp_data[j]));
  }
  client.println("</script>");
}
