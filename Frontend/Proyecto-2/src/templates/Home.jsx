import { useState } from "react";
import { getKanban, getErrors, getReport2, getDot, downloadDot, getTokens } from "../service/api";
import Reporte1 from "./Reporte1";
import Reporte2 from "./Reporte2";
import ErrorTable from "./ErrorTable";
import TokenTable from "./TokenTable";
import "../styles/home.css";

export default function Home() {

    const [input, setInput] = useState("");
    const [data, setData] = useState(null);
    const [errores, setErrores] = useState([]);
    const [tokens, setTokens] = useState([]);
    const [backendError, setBackendError] = useState(null);

    const [reporte, setReporte] = useState(false);
    const [reporte2, setReporte2] = useState(false);
    const [dataReporte2, setDataReporte2] = useState(null);
    const [dot, setDot] = useState("");

    // Cargar archivo .task
    const cargarArchivo = (e) => {
        const file = e.target.files[0];
        if (!file) return;

        const reader = new FileReader();
        reader.onload = (event) => {
            setInput(event.target.result);
        };
        reader.readAsText(file);
    };

    // Ver el árbol DOT
    const verDot = async () => {
        const text = await getDot();
        setDot(text);
    };

    // Analizar el archivo - muestra ambas tablas automáticamente
    const analizar = async () => {
        if (input.trim() === "") {
            setBackendError({ mensaje: "Area de Carga Vacia" });
            setErrores([]);
            setTokens([]);
            setData(null);
            return;
        }

        try {
            // Limpiar reportes anteriores
            setReporte(false);
            setReporte2(false);
            setDot("");
            
            // Obtener datos del backend
            const response = await getKanban(input);
            const erroresRes = await getErrors(input);
            const tokensRes = await getTokens(input);

            // Procesar errores
            let erroresTotales = [];
            if (erroresRes?.errores) {
                erroresTotales = erroresTotales.concat(
                    erroresRes.errores.map(e => ({
                        ...e,
                        tipo: e.tipo || "Léxico",
                        gravedad: e.gravedad || "ERROR"
                    }))
                );
            }
            setErrores(erroresTotales);
            
            // Procesar tokens
            if (tokensRes?.tokens) {
                setTokens(tokensRes.tokens);
            } else {
                setTokens([]);
            }

            if (response.estado === "error" || response.error) {
                setData(null);
                setBackendError({
                    mensaje: response.mensaje || response.error
                });
                return;
            }

            setData(response);
            setBackendError(null);

        } catch (err) {
            console.error(err);
            setBackendError({ mensaje: "Error de conexión con backend" });
            setErrores([]);
            setTokens([]);
            setData(null);
        }
    };

    // Reporte 1 (Kanban)
    const mostrarReporte = () => {
        if (!data) return;
        setReporte(true);
        setReporte2(false);
    };

    // Reporte 2 (Carga por responsable)
    const generarReporte2 = async () => {
        if (input.trim() === "") {
            setBackendError({ mensaje: "Area de Carga Vacia" });
            return;
        }

        try {
            const res = await getReport2(input);
            setDataReporte2(res);
            setReporte(false);
            setReporte2(true);
            setBackendError(null);
        } catch (error) {
            console.error(error);
            setBackendError({ mensaje: "Error de conexión con backend" });
        }
    };

    return (
        <div className="home-container">
            <div className="titulo-div">
                <h1 className="titulo">
                    Proyecto 2 - Universidad de San Carlos de Guatemala
                </h1>
            </div>

            <h2 className="home-title">Analizador Kanban</h2>

            {/* Textarea arriba del todo */}
            <div className="textarea-section">
                <textarea
                    className="home-textarea"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Escribe o pega tu tablero aquí..."
                />
            </div>

            {/* Las dos tablas juntas - se muestran automáticamente después de analizar */}
            <div className="tables-section">
                <ErrorTable errores={errores} />
                <TokenTable tokens={tokens} />
            </div>

            {/* Botones */}
            <div className="home-buttons">
                <input type="file" accept=".task,.txt" onChange={cargarArchivo} style={{ display: "none" }} id="fileInput" />
                <label htmlFor="fileInput" className="home-button file-button">
                     Archivo
                </label>

                <button className="home-button" onClick={analizar}>
                     Analizar
                </button>

                <button className="home-button" onClick={mostrarReporte}>
                    Reporte 1
                </button>

                <button className="home-button" onClick={generarReporte2}>
                    Reporte 2
                </button>

                <button className="home-button" onClick={verDot}>
                    Ver Arbol DOT
                </button>

                <button className="home-button" onClick={downloadDot}>
                     Descargar .DOT
                </button>
            </div>

            {/* Mensajes y reportes */}
            <div className="home-messages">

                 {backendError && (
                    <div className="home-error">
                        <h4>Error:</h4>
                        <p>{backendError.mensaje}</p>
                    </div>
                )}
       
                {reporte && data && (
                    <div className="home-report">
                        <Reporte1 data={data} />
                    </div>
                )}

                {reporte2 && dataReporte2 && (
                    <div className="home-report">
                        <Reporte2 data={dataReporte2} />
                    </div>
                )}

                {dot && (
                    <div className="home-report">
                        <h3>Arbol DOT: Copialo y Pegalo en Graphviz</h3>
                        <textarea 
                            value={dot}
                            readOnly
                            rows={20}
                            style={{ width: "100%" }}
                        />
                    </div>
                )}
            </div>
        </div>
    );
}