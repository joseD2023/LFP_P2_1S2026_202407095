import "../styles/home.css";

export default function TokenTable({ tokens }) {

    return (
        <div className="token-table-container">
            <h3> Tokens Válidos</h3>
            <div className="table-scroll">
                <table className="token-table">
                    <thead>
                        <tr>
                            <th>No</th>
                            <th>Lexema</th>
                            <th>Tipo</th>
                            <th>Fila</th>
                            <th>Columna</th>
                        </tr>
                    </thead>
                    <tbody>
                        {tokens && tokens.length > 0 ? (
                            tokens.map((token, i) => (
                                <tr key={i}>
                                    <td>{token.no || i + 1}</td>
                                    <td style={{ textAlign: "left", fontFamily: "monospace" }}>
                                        {token.lexema}
                                    </td>
                                    <td>{token.tipo}</td>
                                    <td>{token.fila}</td>
                                    <td>{token.columna}</td>
                                </tr>
                            ))
                        ) : (
                            <tr>
                                <td colSpan="5" style={{ textAlign: "center" }}>
                                   tokens Aun No encontrados 
                                </td>
                            </tr>
                        )}
                    </tbody>
                </table>
            </div>
        </div>
    );
}