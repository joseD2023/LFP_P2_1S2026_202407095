import { BrowserRouter, Routes, Route } from "react-router-dom";
import Api from "../templates/api";
import Home from "../templates/home";

export default function AppRouter() {
    return (
        <BrowserRouter>
            <Routes>
                <Route path="/" element={<Home />} />
                <Route path="/analyze" element={<Api />} />
                
            </Routes>
        </BrowserRouter>
    );
}