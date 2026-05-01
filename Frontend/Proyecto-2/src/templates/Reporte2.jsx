import "../styles/reporte2.css";

export default function Reporte2({ data }) {

    if (!data || !data.reporte2) return null;

    return (
        <div className="reporte2-container">
            <h3 className="reporte2-title">Reporte 2 - Carga por Responsable</h3>

            <table className="reporte2-table">
                <thead>
                    <tr>
                        <th>Responsable</th>
                        <th>Total</th>
                        <th>ALTA</th>
                        <th>MEDIA</th>
                        <th>BAJA</th>
                        <th>%</th>
                    </tr>
                </thead>

                <tbody>
                    {data.reporte2.map((r, index) => (
                        <tr key={index}>
                            <td>{r.responsable}</td>
                            <td>{r.total}</td>
                            <td className="alta">{r.ALTA}</td>
                            <td className="media">{r.MEDIA}</td>
                            <td className="baja">{r.BAJA}</td>
                            <div className="barra-container">
                                <div className="barra"style={{ width: `${r.porcentaje}%` }}>
                                    <td>{r.porcentaje}%</td>

                                </div>
                            </div>
                            
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
}