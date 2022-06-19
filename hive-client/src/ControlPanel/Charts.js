import {
     Box,
     FormControl,
     InputLabel,
     Select,
     MenuItem,
     Typography
 } from "@mui/material"
import { useState } from "react"

export const Charts = ({ data }) => {
    console.log('<<=|X|=>> ~ file: Charts.js ~ line 11 ~ Charts ~ data', data);
    const [hive, setHive] = useState(1);

    return (
        <Box>
            <FormControl fullWidth>
                <InputLabel id="demo-simple-select-label">Hive</InputLabel>
                <Select
                    labelId="demo-simple-select-label"
                    id="demo-simple-select"
                    value={hive}
                    label="Age"
                    onChange={event => setHive(event.target.value)}
                >
                    <MenuItem value={1}>One</MenuItem>
                    <MenuItem value={2}>Two</MenuItem>
                    <MenuItem value={3}>Three</MenuItem>
                    <MenuItem value={4}>Four</MenuItem>
                    <MenuItem value={5}>Five</MenuItem>
                    <MenuItem value={6}>Six</MenuItem>
                    <MenuItem value={7}>Seven</MenuItem>
                    <MenuItem value={8}>Eight</MenuItem>
                </Select>
            </FormControl>
            <Typography>
                Temperature: {data[0].fields[`tempc${hive}`]}
            </Typography>
            <Typography>
                Humidity: {data[0].fields[`humidity${hive}`]}
            </Typography>
        </Box>
    )
}