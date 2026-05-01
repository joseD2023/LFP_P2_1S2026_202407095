export default function Kanban({ data }) {

    return (
        <div style={{
            display: "flex",
            gap: "15px",
            marginTop: "20px"
        }}>

            {data?.tablero?.columnas?.map((col, i) => (
                <div key={i} style={{
                    minWidth: "250px",
                    background: "#f4f4f4",
                    padding: "10px",
                    borderRadius: "10px"
                }}>

                    <h3 style={{ textAlign: "center" }}>
                        {col.nombre}
                    </h3>

                    {col.tareas?.map((t, j) => {

                        let color = "#ccc";

                        if (t.prioridad === "ALTA") color = "red";
                        if (t.prioridad === "MEDIA") color = "orange";
                        if (t.prioridad === "BAJA") color = "green";

                        return (
                            <div key={j} style={{
                                background: "white",
                                margin: "10px 0",
                                padding: "10px",
                                borderRadius: "8px",
                                borderLeft: `6px solid ${color}`
                            }}>
                                <h4>{t.nombre}</h4>
                                <p>{t.responsable}</p>
                                <p>{t.fecha_limite}</p>
                            </div>
                        );
                    })}

                </div>
            ))}

        </div>
    );
}