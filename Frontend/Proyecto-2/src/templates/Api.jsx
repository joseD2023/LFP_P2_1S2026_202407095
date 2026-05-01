import { useEffect, useState } from "react";
import { getKanban } from "../service/api";
import "../styles/Api.css"


export default function Api() {

    const [data, setData] = useState(null);
    const [error, setError] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {

        const fetchData = async () => {
            try {

                const response = await getKanban(`
TABLERO "Mi tablero" {
    COLUMNA "Pendientes" {
        tarea: "Hacer proyecto" [prioridad: ALTA, responsable: "Jose", fecha_limite: 2026-05-01],
        tarea: "Diseñar parser" [prioridad: MEDIA, responsable: "Ana", fecha_limite: 2026-05-03]
    };

    COLUMNA "En Progreso" {
        tarea: "Frontend React" [prioridad: ALTA, responsable: "Luis", fecha_limite: 2026-05-02]
    };

    COLUMNA "Completado" {
        tarea: "Configurar C++ backend" [prioridad: BAJA, responsable: "Carlos", fecha_limite: 2026-04-28]
    };
};
                `);

                if (response.status === "error") {
                    setError(response.mensaje || response.Mensaje);
                } else {
                    setData(response);
                }

            } catch (err) {
                setError("Error de conexión con el servidor");
            } finally {
                setLoading(false);
            }
        };

        fetchData();

    }, []);

    if (loading) {
        return (
            <div className="loading-container">
                <h2>Cargando tablero...</h2>
            </div>
        );
    }

    if (error) {
        return (
            <div className="error-container">
                <h2>Error: {error}</h2>
            </div>
        );
    }

    return (
        <div className="kanban-container">

            {data?.tablero?.columnas?.map((col, i) => (
                <div key={i} className="kanban-column">

                    <h3 className="kanban-title">
                        {col.nombre}
                    </h3>

                    {col.tareas?.map((t, j) => {

                        let priorityClass = "";

                        if (t.prioridad === "ALTA") priorityClass = "high";
                        if (t.prioridad === "MEDIA") priorityClass = "medium";
                        if (t.prioridad === "BAJA") priorityClass = "low";

                        return (
                            <div
                                key={j}
                                className={`task-card ${priorityClass}`}
                            >
                                <h4>{t.nombre}</h4>
                                <p><b>Prioridad:</b> {t.prioridad}</p>
                                <p><b>Responsable:</b> {t.responsable}</p>
                                <p><b>Fecha:</b> {t.fecha_limite}</p>
                            </div>
                        );
                    })}

                </div>
            ))}

        </div>
    );
}