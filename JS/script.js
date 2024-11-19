function controlComponent(part, increment) {
    fetch(`PHP/control.php?get_status=${part}`)
        .then(response => response.json())
        .then(data => {
            let currentStatus = data.status;
            let newStatus = calculateNewStatus(part, currentStatus, increment);
            if (newStatus !== null) {
                console.log(`Actualizando ${part} de ${currentStatus} a ${newStatus}`);
                updateControlStatus(part, newStatus);
            } else {
                console.log(`Valor fuera de límites para ${part}: ${newStatus}`);
            }
        })
        .catch(error => console.error(`Error al obtener el estado de ${part}:`, error));
}


function calculateNewStatus(part, currentStatus, increment) {
    let newStatus = currentStatus + increment;
    const limits = {
        'motor': [-1, 1],
        'servodirection': [-1, 1],
        'servoaxis': [-6, 6],
        'servolift': [-3, 3],
        'servogrip': [-5, 0]
    };

    if (limits[part]) {
        const [min, max] = limits[part];
        return newStatus >= min && newStatus <= max ? newStatus : null;
    }
    return null;
}


function updateControlStatus(part, newStatus) {
    fetch('PHP/control.php', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ part, status: newStatus })
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            console.log(`${part} status updated to ${newStatus}`);
            updateButtonStyles(part, newStatus);
        } else {
            console.error(`Error updating ${part} status:`, data.error);
        }
    })
    .catch(error => console.error(`Error al actualizar el estado de ${part}:`, error));
}


function updateButtonStyles(part, newStatus) {
    const buttonMap = {
        'motor': ['btnW', 'btnS'],
        'servodirection': ['btnA', 'btnD'],
        'servoaxis': ['leftAxis', 'rightAxis'],
        'servolift': ['tiltDown', 'tiltUp'],
        'servogrip': ['closeGrip', 'openGrip']
    };

    const buttons = buttonMap[part];

    if (newStatus < 0) {
        document.getElementById(buttons[0]).classList.add('active');
        document.getElementById(buttons[1]).classList.remove('active');
    } else if (newStatus > 0) {
        document.getElementById(buttons[0]).classList.remove('active');
        document.getElementById(buttons[1]).classList.add('active');
    } else {
        document.getElementById(buttons[0]).classList.remove('active');
        document.getElementById(buttons[1]).classList.remove('active');
    }
}


document.getElementById('btnW').onclick = () => controlComponent('motor', 1);
document.getElementById('btnS').onclick = () => controlComponent('motor', -1);
document.getElementById('btnA').onclick = () => controlComponent('servodirection', -1);
document.getElementById('btnD').onclick = () => controlComponent('servodirection', 1);
document.getElementById('leftAxis').onclick = () => controlComponent('servoaxis', -1);
document.getElementById('rightAxis').onclick = () => controlComponent('servoaxis', 1);
document.getElementById('tiltDown').onclick = () => controlComponent('servolift', -1);
document.getElementById('tiltUp').onclick = () => controlComponent('servolift', 1);
document.getElementById('closeGrip').onclick = () => controlComponent('servogrip', -1);
document.getElementById('openGrip').onclick = () => controlComponent('servogrip', 1);


function updateDHT22Data() {
    fetch('PHP/dht22.php')
        .then(response => response.json())
        .then(data => {
            document.getElementById('temperature').innerText = data.temperature;
            document.getElementById('humidity').innerText = data.humidity;
        })
        .catch(error => console.error('Error al obtener los datos del sensor DHT22:', error));
}
setInterval(updateDHT22Data, 5000);


function updateVideoStream() {
    const video = document.getElementById('videoStream');
    video.src = `http://x.x.x.x/stream?${new Date().getTime()}`;
}
setInterval(updateVideoStream, 5000);