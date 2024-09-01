function handleResponse(response) {
    if (!response.ok) {
        throw new Error('Network response was not ok');
    }
    return response.json();
}

function displayMessage(message, isError = false) {
    const messageBox = document.getElementById('messageBox');
    messageBox.textContent = message;
    messageBox.className = 'message-box'; // 重置类
    if (isError) {
        messageBox.classList.add('error');
    } else {
        messageBox.classList.add('success');
    }
    messageBox.classList.add('show');

    setTimeout(() => {
        messageBox.classList.remove('show');
        messageBox.classList.add('hide');
    }, 3000); // 消息显示3秒后消失
}

function handleError(error) {
    console.error('Fetch error:', error);
    displayMessage('错误: ' + error.message, true); 
}

function insertData() {
    const id = document.getElementById('insertId').value;
    const name = document.getElementById('insertName').value;
    const score1 = document.getElementById('insertScore1').value;
    const score2 = document.getElementById('insertScore2').value;
    console.log(id, name, score1, score2); 
    fetch('/insert', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id, name, score1, score2 })
    })
    .then(handleResponse)
    .then(data => {
        displayMessage(data.message);
        queryData();
        displayTotalCount();
    })
    .catch(handleError);
}

function deleteData() {
    const id = document.getElementById('deleteId').value;
    fetch('/delete', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id })
    })
    .then(handleResponse)
    .then(data => {
        if (data.error) {
            displayMessage('错误: ' + data.error, true);
        } else {
            displayMessage(data.message);
            queryData();
            displayTotalCount();
        }
    })
    .catch(handleError);
}

function queryData() {
    fetch('/query')
        .then(handleResponse)
        .then(data => {
            const table = $('#queryResult').DataTable();
            table.clear().rows.add(data).draw();
        })
        .catch(handleError);
}

function updateData() {
    const id = document.getElementById('updateId').value;
    const name = document.getElementById('updateName').value;
    const score1 = document.getElementById('updateScore1').value;
    const score2 = document.getElementById('updateScore2').value;
    fetch('/update', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id, name, score1, score2 })
    })
    .then(handleResponse)
    .then(data => {
        displayMessage(data.message);
        queryData();
        displayTotalCount();
    })
    .catch(handleError);
}

function sortDataByScore(scoreColumn, sortOrder) {
    fetch(`/sort?scoreColumn=${scoreColumn}&sortOrder=${sortOrder}`)
        .then(handleResponse)
        .then(data => {
            const table = $('#queryResult').DataTable();
            table.clear().rows.add(data).draw();
        })
        .catch(handleError);
}

function findStudentById(id) {
    fetch(`/find?id=${id}`)
        .then(handleResponse)
        .then(data => {
            const table = $('#queryResult').DataTable();
            table.clear().rows.add(data).draw();
        })
        .catch(handleError);
}

function displayTotalCount() {
    fetch('/count')
        .then(handleResponse)
        .then(data => document.getElementById('totalCount').value = data.count)
        .catch(handleError);
}

$(document).ready(function() {
    $('#queryResult').DataTable({
        columns: [
            { data: 'id' },
            { data: 'name' },
            { data: 'score1' },
            { data: 'score2' },
            { data: 'changeTime' }
        ],
        paging: true,
        searching: false,
        ordering: false,
    });
    queryData();
});

const canvas = document.getElementById('particleCanvas');
const ctx = canvas.getContext('2d');
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

function Particle(x, y) {
    this.x = x;
    this.y = y;
    this.radius = Math.random() * 5 + 1; // 可以调整大小范围以改变效果
    this.speedX = Math.random() * 2 - 1; // 增加速度范围
    this.speedY = Math.random() * 2 - 1; // 增加速度范围

    this.update = function() {
        this.x += this.speedX;
        this.y += this.speedY;

        // Bounce particles off the edges
        if (this.x < 0 || this.x > canvas.width) this.speedX *= -1;
        if (this.y < 0 || this.y > canvas.height) this.speedY *= -1;

        this.draw();
    };

    this.draw = function() {
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2, false);
        ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
        ctx.fill();
    };
}

let particles = [];
for (let i = 0; i < 50; i++) {
    let x = Math.random() * canvas.width;
    let y = Math.random() * canvas.height;
    particles.push(new Particle(x, y));
}

function animate() {
    requestAnimationFrame(animate);
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    particles.forEach(particle => {
        particle.update();
    });
}

animate();