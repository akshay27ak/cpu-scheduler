const { ipcRenderer } = require('electron');

const submitBtn = document.getElementById('submit');
const resultElement = document.getElementById('result');

submitBtn.addEventListener('click', () => {
    const algorithmInput = document.getElementById('algorithm');
    const processIdInput = document.getElementById('process-id');
    const arrivalTimeInput = document.getElementById('arrival-time');
    const burstTimeInput = document.getElementById('burst-time');
    const priorityInput = document.getElementById('priority');
    const timeQuantumInput = document.getElementById('time-quantum');
  
    const data = {
        algorithm: algorithmInput.value,
        processId: processIdInput.value,
        arrivalTime: arrivalTimeInput.value,
        burstTime: burstTimeInput.value,
        priority: priorityInput ? priorityInput.value : null,
        timeQuantum: timeQuantumInput ? timeQuantumInput.value : null,
    };

    console.log("User Input data", data);

    // Send data to main process
    ipcRenderer.send('run-scheduler', data);
});

ipcRenderer.on('scheduler-output', (event, data) => {
    if (data.error) {
        resultElement.innerHTML = `<p>Error: ${data.error}</p>`;
    } else {
        resultElement.innerHTML = data.tableHtml;
    }
});
