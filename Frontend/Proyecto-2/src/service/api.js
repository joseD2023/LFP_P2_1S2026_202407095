const API_URL = "http://localhost:8080/analyze";
const REPORT_URL = "http://localhost:8080/report1";
const REPORT_ERRORES = "http://localhost:8080/report3"; 


/*Consumimos la parte del los datos analizados del servidor en c++ */
export const getKanban = async (texto) => {
    try {
        const response = await fetch(API_URL, {
            method: "POST",
            headers: {
                "Content-Type": "text/plain"
            },
            body: texto
        });

        if (!response.ok) {
            throw new Error("Error HTTP: " + response.status);
        }

        const data = await response.json();
        return data;

    } catch (error) {
        console.log("Error en el Servidor", error);
    }
};




/*Consumimos la api para el reporte 1*/

export const report1 = async (texto) => {
    try {
        const response = await fetch(REPORT_URL, {
            method: "POST",
            headers: {
                "Content-Type": "text/plain"
            },
            body: texto
        });

        if (!response.ok) {
            throw new Error("Error HTTP: " + response.status);
        }

        const data = await response.json();
        return data;

    } catch (error) {
        console.log("Error en reporte 1", error);
    }
};



/*Ahora vamos a consumir los recursos de lo que son mi enpoint */
export const getErrors = async (texto) => {
    try {
        const response = await fetch(REPORT_ERRORES, {
            method: "POST",
            headers: { "Content-Type": "text/plain" },
            body: texto
        });

        const text = await response.text();

        let data;
        try {
            data = JSON.parse(text);
        } catch (e) {
            return {
                status: "error",
                mensaje: "Respuesta inválida del servidor",
                raw: text
            };
        }

        return data;

    } catch (error) {
        return {
            status: "error",
            mensaje: "Error de conexión: " + error.message
        };
    }
};



/*Ahora vamos con el siguiente reporte que es el 2 */

export const getReport2 = async (texto) => {
    const response = await fetch("http://localhost:8080/report2", {
        method: "POST",
        headers: {
            "Content-Type": "text/plain"
        },
        body: texto
    });

    return await response.json();
};


/* Colocaremos un consumo de api para descargar el archivo .DOT del arbol y asi visualizarlo en una Graphiz */

export const downloadDot = async () => {
    const res = await fetch("http://localhost:8080/download-dot");
    const blob = await res.blob();

    const url = window.URL.createObjectURL(blob);
    const a = document.createElement("a");

    a.href = url;
    a.download = "arbol.dot";
    a.click();

    window.URL.revokeObjectURL(url);
};


export const getDot = async () => {
    const res = await fetch("http://localhost:8080/download-dot");
    return await res.text();
};