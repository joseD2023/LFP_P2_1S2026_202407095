export default function ErrorTable({ errores }) {

    return (
        <div className="error-table-container">

            <h3>Errores Léxicos</h3>

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
                                <td>{i + 1}</td>
                                <td>{e.lexema}</td>
                                <td>Léxico</td>
                                <td>{e.descripcion}</td>
                                <td>{e.fila}</td>
                                <td>{e.columna}</td>
                            </tr>
                        ))
                    ) : (
                        <tr>
                            <td colSpan="6">Sin errores</td>
                        </tr>
                    )}
                </tbody>

            </table>

        </div>
    );
}