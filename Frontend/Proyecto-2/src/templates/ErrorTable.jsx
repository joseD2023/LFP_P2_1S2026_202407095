import "../styles/home.css";

export default function ErrorTable({ errores }) {

    return (
        <div className="error-table-container">
            <h3> Errores del Análisis</h3>
            <div className="table-scroll">
                <table className="error-table">
                    <thead>
                        <tr>
                            <th>No</th>
                            <th>Lexema</th>
                            <th>Tipo</th>
                            <th>Descripcion</th>
                            <th>Fila</th>
                            <th>Columna</th>
                        </tr>
                    </thead>
                    <tbody>
                        {errores && errores.length > 0 ? (
                            errores.map((e, i) => (
                                <tr key={i}>
                                    <td>{e.no || i + 1}</td>
                                    <td style={{ textAlign: "left", fontFamily: "monospace" }}>
                                        {e.lexema}
                                    </td>
                                    <td style={{ 
                                        color: e.tipo === "Sintáctico" ? "#c0392b" : "#e67e22",
                                        fontWeight: "bold"
                                    }}>
                                        {e.tipo || "Léxico"}
                                    </td>
                                    <td style={{ textAlign: "left" }}>{e.descripcion}</td>
                                    <td>{e.fila}</td>
                                    <td>{e.columna}</td>
                                </tr>
                            ))
                        ) : (
                            <tr>
                                <td colSpan="6" style={{ textAlign: "center" }}>
                                     Sin errores
                                </td>
                            </tr>
                        )}
                    </tbody>
                </table>
            </div>
        </div>
    );
}