const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Hatch - Panel de Control</title>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap" rel="stylesheet">
    <style>
        :root { 
            --bg-color: #0f172a; 
            --card-bg: rgba(30, 41, 59, 0.7); 
            --accent-color: #38bdf8; 
            --text-color: #f1f5f9; 
            --success: #22c55e; 
            --error: #ef4444; 
        }
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Poppins', sans-serif; 
            background-color: var(--bg-color); 
            color: var(--text-color); 
            display: flex; 
            justify-content: center; 
            padding: 20px; 
        }
        .dashboard { width: 100%; max-width: 900px; }
        header { text-align: center; margin-bottom: 30px; }
        header h1 { color: var(--accent-color); letter-spacing: 2px; }
        
        .monitor-grid { 
            display: grid; 
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); 
            gap: 15px; 
            margin-bottom: 30px; 
        }
        .stat-card { 
            background: linear-gradient(145deg, rgba(30, 41, 59, 0.9), rgba(15, 23, 42, 0.9)); 
            border: 1px solid var(--accent-color); 
            border-radius: 15px; 
            padding: 15px; 
            text-align: center; 
        }
        .stat-value { font-size: 1.8rem; font-weight: 600; display: block; }
        .stat-label { font-size: 0.7rem; text-transform: uppercase; color: var(--accent-color); letter-spacing: 1px; }
        
        .btn-action { 
            width: 100%; 
            padding: 12px; 
            border-radius: 10px; 
            font-weight: 600; 
            cursor: pointer; 
            transition: 0.3s; 
            border: none; 
            color: white; 
            margin-top: 10px; 
        }
        .btn-start { background: var(--success); }
        .btn-stop { background: var(--error); }
        
        .grid-layout { 
            display: grid; 
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); 
            gap: 20px; 
        }
        .card { 
            background: var(--card-bg); 
            backdrop-filter: blur(10px); 
            border: 1px solid rgba(255,255,255,0.1); 
            border-radius: 20px; 
            padding: 20px; 
        }
        .card-title { 
            font-weight: 600; 
            color: var(--accent-color); 
            margin-bottom: 15px; 
            border-bottom: 1px solid rgba(255,255,255,0.1); 
            padding-bottom: 8px; 
        }
        label { display: block; font-size: 0.75rem; color: #94a3b8; margin-bottom: 4px; }
        input { 
            width: 100%; 
            background: rgba(15, 23, 42, 0.8); 
            border: 1px solid #334155; 
            border-radius: 8px; 
            padding: 10px; 
            color: white; 
            outline: none; 
            margin-bottom: 10px; 
        }
        input:invalid { border-color: var(--error); }
        .flex-inputs { display: flex; gap: 10px; }
        .btn-save { 
            width: 100%; 
            background: linear-gradient(135deg, #334155 0%, #1e293b 100%); 
            color: white; 
            border: 1px solid var(--accent-color); 
            padding: 15px; 
            border-radius: 12px; 
            font-weight: 600; 
            cursor: pointer; 
            margin-top: 20px; 
        }
        ::-webkit-calendar-picker-indicator { filter: invert(1); }
    </style>
</head>
<body>
<div class="dashboard">
    <header><h1>INCUBADORA PRO</h1><p>Centro de Monitoreo</p></header>
    
    <div class="monitor-grid">
        <div class="stat-card">
            <span class="stat-label">Temperatura</span>
            <span class="stat-value">%TEMP_ACTUAL%°C</span>
        </div>
        <div class="stat-card">
            <span class="stat-label">Humedad</span>
            <span class="stat-value">%HUM_ACTUAL%%</span>
        </div>
        <div class="stat-card">
            <span class="stat-label">Tiempo Transcurrido</span>
            <span class="stat-value" id="timer">00:00:00</span>
        </div>
        <div class="stat-card">
            <span class="stat-label">Estado: <span id="status-text">INACTIVO</span></span>
            <button id="main-btn" class="btn-action btn-start" onclick="toggleIncubation()">INICIAR CICLO</button>
        </div>
    </div>

    <div class="grid-layout">
        <div class="card">
            <div class="card-title">📅 Ciclo</div>
            <label>Fecha de Finalización</label>
            <input type="datetime-local" id="fecha_fin" value="%FECHA_FIN%" required>
        </div>
        
        <div class="card">
            <div class="card-title">🌡️ Clima</div>
            <label>Temp (°C) Mín / Máx (5 - 95)</label>
            <div class="flex-inputs">
                <input type="number" id="t_min" step="0.1" min="5" max="95" value="%T_MIN%">
                <input type="number" id="t_max" step="0.1" min="5" max="95" value="%T_MAX%">
            </div>
            <label>Hum (%) Mín / Máx (0 - 100)</label>
            <div class="flex-inputs">
                <input type="number" id="h_min" min="0" max="100" value="%H_MIN%">
                <input type="number" id="h_max" min="0" max="100" value="%H_MAX%">
            </div>
        </div>
        
        <div class="card">
            <div class="card-title">🔄 Volteo</div>
            <label>Intervalo (1 - 24 Horas)</label>
            <input type="number" id="rot_int" min="1" max="24" value="%R_INT%">
            <label>Duración (1 - 300 Seg)</label>
            <input type="number" id="rot_dur" min="1" max="300" value="%R_DUR%">
        </div>
    </div>
    
    <button class="btn-save" onclick="saveConfig()">GUARDAR CONFIGURACIÓN</button>
</div>

<script>
    let running = false;
    let startTime = 0;
    let timerInterval;

    window.onload = function() {
        // Bloquear fechas pasadas en el selector
        const now = new Date();
        now.setMinutes(now.getMinutes() - now.getTimezoneOffset());
        document.getElementById('fecha_fin').min = now.toISOString().slice(0,16);

        const startFromESP = Number("%START_TIME_VAL%"); 
        if(startFromESP > 0) {
            updateUI(true, startFromESP);
        }
    };

    function toggleIncubation() {
        const cmd = !running ? "start" : "stop";
        const now = Date.now();
        
        if(confirm(`¿Desea ${cmd === 'start' ? 'iniciar' : 'detener'} la incubación?`)) {
            fetch('/update', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({ cmd: cmd, ts: now })
            }).then(res => {
                if(res.ok) {
                    if(!running) updateUI(true, now);
                    else updateUI(false);
                } else {
                    alert("Error en la comunicación con la incubadora");
                }
            });
        }
    }

    function saveConfig() {
        const fechaFinVal = new Date(document.getElementById('fecha_fin').value);
        const ahora = new Date();

        // Validación de fecha
        if (isNaN(fechaFinVal.getTime()) || fechaFinVal <= ahora) {
            alert("Error: La fecha de finalización debe ser posterior a la actual.");
            return;
        }

        // Validación de rangos numéricos (HTML min/max)
        const inputs = document.querySelectorAll('input[type="number"]');
        for (let input of inputs) {
            if (!input.checkValidity()) {
                alert(`Valor no válido en ${input.id}: debe estar entre ${input.min} y ${input.max}`);
                return;
            }
        }

        const config = {
            cmd: "save",
            fin: document.getElementById('fecha_fin').value,
            t_mi: parseFloat(document.getElementById('t_min').value),
            t_ma: parseFloat(document.getElementById('t_max').value),
            h_mi: parseInt(document.getElementById('h_min').value),
            h_ma: parseInt(document.getElementById('h_max').value),
            r_i: parseInt(document.getElementById('rot_int').value),
            r_d: parseInt(document.getElementById('rot_dur').value)
        };
        
        fetch('/update', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify(config)
        }).then(res => {
            if(res.ok) alert("Configuración Guardada Correctamente");
            else alert("Error al guardar la configuración");
        });
    }

    function updateUI(isRunning, time = 0) {
        running = isRunning;
        const btn = document.getElementById('main-btn');
        const statusTxt = document.getElementById('status-text');

        if(running) {
            startTime = time;
            statusTxt.innerText = "ACTIVO";
            statusTxt.style.color = "var(--success)";
            btn.innerText = "PARAR INCUBACIÓN";
            btn.className = "btn-action btn-stop";
            timerInterval = setInterval(runTimer, 1000);
            runTimer();
        } else {
            clearInterval(timerInterval);
            statusTxt.innerText = "INACTIVO";
            statusTxt.style.color = "white";
            btn.innerText = "INICIAR CICLO";
            btn.className = "btn-action btn-start";
            document.getElementById('timer').innerText = "00:00:00";
        }
    }

    function runTimer() {
        const diff = Math.floor((Date.now() - startTime) / 1000);
        if (diff < 0) return;
        const h = Math.floor(diff / 3600).toString().padStart(2, '0');
        const m = Math.floor((diff % 3600) / 60).toString().padStart(2, '0');
        const s = (diff % 60).toString().padStart(2, '0');
        document.getElementById('timer').innerText = `${h}:${m}:${s}`;
    }
</script>
</body>
</html>
)rawliteral";