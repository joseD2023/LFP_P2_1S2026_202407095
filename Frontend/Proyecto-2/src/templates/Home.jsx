import { useState } from "react";
import { getKanban, getErrors } from "../service/api";
import Reporte1 from "./Reporte1";
import ErrorTable from "./ErrorTable";
import "../styles/home.css";

export default function Home() {

    const [input, setInput] = useState("");
    const [data, setData] = useState(null);
    const [errores, setErrores] = useState([]);
    const [backendError, setBackendError] = useState(null);
    const [reporte, setReporte] = useState(false);

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

       
            if (response.estado === "error" || response.error) {

                setData(null);
                setReporte(false);

                setBackendError({
                    mensaje: response.mensaje || response.error
                });

                setErrores(erroresRes?.errores || []);

                return;
            }

            
            setData(response);
            setErrores(erroresRes?.errores || []);
            setBackendError(null);
            setReporte(false);

        } catch (err) {

            setBackendError({ mensaje: "Error de conexión con backend" });
            setErrores([]);
            setData(null);
        }
    };

    const mostrarReporte = () => {
        if (!data) return;
        setReporte(true);
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

                {/* Pasamos los datos para la formacion del tablero*/}
                <ErrorTable errores={errores} />

            </div>

            {/* BOTONES */}
            <div className="home-buttons">

                <button className="home-button" onClick={analizar}>
                    Analizar
                </button>

                <button className="home-button" onClick={mostrarReporte}>
                    Reporte 1
                </button>

            </div>

            {/*resultados en lo que queremos ver de la respuesta del backend  */}
            <div className="home-messages">

                {backendError && (
                    <div className="home-error">
                        <h4>Error:</h4>
                        <p>{backendError.mensaje}</p>
                    </div>
                )}

                {data && !reporte && (
                    <pre className="home-json">
                        {JSON.stringify(data, null, 2)}
                    </pre>
                )}

                {reporte && data && (
                    <div className="home-report">
                        <Reporte1 data={data} />
                    </div>
                )}

            </div>

        </div>
    );
}