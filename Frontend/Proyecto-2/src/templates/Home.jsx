import { useState } from "react";
import { getKanban, getErrors, getReport2 } from "../service/api";
import Reporte1 from "./Reporte1";
import Reporte2 from "./Reporte2";
import ErrorTable from "./ErrorTable";
import "../styles/home.css";

export default function Home() {

    const [input, setInput] = useState("");
    const [data, setData] = useState(null);
    const [errores, setErrores] = useState([]);
    const [backendError, setBackendError] = useState(null);

    const [reporte, setReporte] = useState(false);

    const [reporte2, setReporte2] = useState(false);
    const [dataReporte2, setDataReporte2] = useState(null);

    // 🔥 ANALIZAR
    const analizar = async () => {

        if (input.trim() === "") {
            setBackendError({ mensaje: "Area de Carga Vacia" });
            setErrores([]);
            setData(null);
            return;
        }

        try {

            const response = await getKanban(input);
            const erroresRes = await getErrors(input);

            let erroresTotales = [];

            // 🔴 errores léxicos
            if (erroresRes?.errores) {
                erroresTotales = erroresTotales.concat(
                    erroresRes.errores.map(e => ({
                        ...e,
                        tipo: "Léxico",
                        gravedad: e.gravedad || "ERROR"
                    }))
                );
            }

            setErrores(erroresTotales);

            if (response.estado === "error" || response.error) {

                setData(null);
                setReporte(false);
                setReporte2(false);
                setDataReporte2(null);

                setBackendError({
                    mensaje: response.mensaje || response.error
                });

                return;
            }

            setData(response);
            setBackendError(null);

            setReporte(false);
            setReporte2(false);
            setDataReporte2(null);

        } catch (err) {

            console.error(err);

            setBackendError({ mensaje: "Error de conexión con backend" });
            setErrores([]);
            setData(null);

            setReporte(false);
            setReporte2(false);
            setDataReporte2(null);
        }
    };

    // reporte 1
    const mostrarReporte = () => {
        if (!data) return;

        setReporte(true);
        setReporte2(false);
    };

    // reporte 2
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

            <div className="editor-section">

                <textarea
                    className="home-textarea"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Escribe o pega tu tablero aquí..."
                />

                <ErrorTable errores={errores} />

            </div>

            <div className="home-buttons">

                <button className="home-button" onClick={analizar}>
                    Analizar
                </button>

                <button className="home-button" onClick={mostrarReporte}>
                    Reporte 1
                </button>

                <button className="home-button" onClick={generarReporte2}>
                    Reporte 2
                </button>

            </div>

            <div className="home-messages">

                {backendError && (
                    <div className="home-error">
                        <h4>Error:</h4>
                        <p>{backendError.mensaje}</p>
                    </div>
                )}

                {data && !reporte && !reporte2 && (
                    <pre className="home-json">
                        {JSON.stringify(data, null, 2)}
                    </pre>
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

            </div>

        </div>
    );
}