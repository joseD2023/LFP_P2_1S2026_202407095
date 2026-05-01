import "../styles/reporte1.css";

export default function Reporte1({ data }) {

    return (
        <div className="kanban-container">


            {data?.tablero?.columnas?.map((col, i) => (
                <div key={i} className="kanban-column">

                    <h3 className="kanban-title">{col.nombre}</h3>

                    {col.tareas?.map((t, j) => (
                        <div
                            key={j}
                            className={`task-card ${t.prioridad}`}
                        >

                      
                            <div className="task-priority-bar">
                                {t.prioridad}
                            </div>

                            <h4 className="task-title">{t.nombre}</h4>

                            <p className="task-text">👤 {t.responsable}</p>
                            <p className="task-text">📅 fecha_limite: {t.fecha_limite}</p>

                        </div>
                    ))}

                </div>
            ))}

        </div>
    );
}