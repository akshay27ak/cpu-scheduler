const { spawn } = require('child_process');
const { ipcMain, app, BrowserWindow } = require('electron');
const path = require('node:path')

function createWindow () {
  let mainWindow = new BrowserWindow({
    height: 700,
    width:800,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false
    }
  })
  mainWindow.loadFile('index.html');

  mainWindow.on('closed', function () {
    mainWindow = null;
  });
}

app.on('ready', createWindow);

ipcMain.on('run-scheduler', (event, data) => {
  const { algorithm, processId, arrivalTime, burstTime, priority, timeQuantum} = data;
  console.log("DATA RECEIVED IN MAIN.JS", data);

  const exePath = path.resolve(__dirname, 'scheduler.exe');
  const child = spawn(exePath, [algorithm, processId, arrivalTime, burstTime, priority, timeQuantum]);

  let outputData = '';

  child.stdout.on('data', (data) => {
      outputData += data.toString();
      console.log("OutData : \n", outputData);
  });

  child.stderr.on('data', (data) => {
      console.error(`stderr: ${data}`);
  });

  child.on('close', (code) => {
      if (code === 0) {
          console.log('Child process exited successfully.');
          
          const tableHtml = formatToTable(outputData);
          event.sender.send('scheduler-output', { tableHtml });
      } else {
          console.error(`Child process exited with code ${code}`);
          event.sender.send('scheduler-output', { error: 'Error running scheduler' });
      }
  });
});

function formatToTable(data) {
  const rows = data.split('\n').filter(row => row.trim().length > 0);
  // const header = rows.shift().split(/\s+/);

  // Define custom headers
  const customHeader = ['Job', 'ArrivalTime', 'BurstTime', 'CompletionTime', 'TurnaroundTime', 'WaitingTime'];

  let tableHtml = `<table border="1" cellpadding="5" cellspacing="0"><thead><tr>`;
  customHeader.forEach(col => {
      tableHtml += `<th>${col}</th>`;
  });
  tableHtml += `</tr></thead><tbody>`;

  rows.forEach((row, index) => {
      const cols = row.split(/\s+/);
      tableHtml += `<tr>`;
      cols.forEach(col => {
          tableHtml += `<td>${col}</td>`;
      });
      tableHtml += `</tr>`;
  });

  // Calculate and append averages
  const totalTurnaroundTime = rows.reduce((sum, row) => sum + parseInt(row.split(/\s+/)[4]), 0);
  const totalWaitingTime = rows.reduce((sum, row) => sum + parseInt(row.split(/\s+/)[5]), 0);
  const numProcesses = rows.length;
  const averageTurnaroundTime = (totalTurnaroundTime / numProcesses).toFixed(4);
  const averageWaitingTime = (totalWaitingTime / numProcesses).toFixed(4);

  tableHtml += `<tr>
    <td colspan="3"></td>
      <td>Averages</td>
      <td>${averageTurnaroundTime}</td>
      <td>${averageWaitingTime}</td>
  </tr>`;

  tableHtml += `</tbody></table>`;
  return tableHtml;
}