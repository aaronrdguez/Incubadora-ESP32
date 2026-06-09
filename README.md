# Smart Hatch - Incubadora Inteligente

Controlador de incubadora de huevos basado en **ESP32**, con monitoreo y control vía web.

## Características

- **Control de temperatura** mediante focos (calefacción) y sensor DHT11
- **Control de humedad** mediante ventilador
- **Rotación automática** de huevos con motor, con intervalo y duración configurables
- **Pantalla LCD 16x2 I2C** con información en tiempo real
- **Encoder rotatorio** para navegar y ajustar parámetros localmente
- **Interfaz web WiFi** (modo Access Point) para monitoreo remoto
- **Alertas sonoras** con melodías (Jurassic Park, Star Wars, Pokémon)
- **Detección de fallos** del sensor con parada de seguridad

## Hardware

| Componente | Pin |
|---|---|
| DHT11 | GPIO22 |
| Focos (calefacción) | GPIO21 |
| Motor rotación | GPIO20 |
| Ventilador | GPIO19 |
| Buzzer | GPIO4 |
| Encoder CLK | GPIO2 |
| Encoder DT | GPIO3 |
| Encoder SW | GPIO15 |
| LCD I2C | SDA:0, SCL:1 |

## Conexión WiFi

La ESP32 crea una red WiFi propia:

- **SSID:** `Incubadora_C6_Final`
- **Password:** `12345678`
- **IP:** `192.168.1.100`

Conéctate y abre `http://192.168.1.100` en tu navegador.

## Licencia

MIT
