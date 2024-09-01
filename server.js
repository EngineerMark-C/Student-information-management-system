const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');

const app = express();
app.use(bodyParser.json());
app.use(express.static('public'));

const executeCommand = (command, res) => {
    console.log(`Executing command: ${command}`);
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`exec error: ${error}`);
            return res.status(500).json({ error: error.message });
        }
        if (stderr) {
            console.error(`stderr: ${stderr}`);
            return res.status(500).json({ error: stderr });
        }
        try {
            const jsonData = JSON.parse(stdout); // Ensure stdout is JSON formatted
            if (jsonData.rowsAffected && jsonData.rowsAffected === 0) {
                res.status(404).json({ error: "No data found to delete" });
            } else {
                res.json(jsonData);
            }
        } catch (e) {
            console.error('Invalid JSON response:', stdout);
            res.status(500).json({ error: 'Invalid JSON response' });
        }
    });
};

app.post('/insert', (req, res) => {
    const { id, name, score1, score2 } = req.body;
    const command = `main insert ${id} "${name}" ${score1} ${score2}`;
    executeCommand(command, res);
});

app.post('/delete', (req, res) => {
    const { id } = req.body;
    const command = `main delete ${id}`;
    executeCommand(command, res);
});

app.get('/query', (req, res) => {
    const command = `main query`;
    executeCommand(command, res);
});

app.post('/update', (req, res) => {
    const { id, name, score1, score2 } = req.body;
    const command = `main update ${id} "${name}" ${score1} ${score2}`;
    executeCommand(command, res);
});

app.get('/sort', (req, res) => {
    const { scoreColumn, sortOrder } = req.query;
    const command = `main sort ${scoreColumn} ${sortOrder}`;
    executeCommand(command, res);
});

app.get('/find', (req, res) => {
    const { id } = req.query;
    const command = `main find ${id}`;
    executeCommand(command, res);
});

app.get('/count', (req, res) => {
    const command = `main count`;
    executeCommand(command, res);
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
